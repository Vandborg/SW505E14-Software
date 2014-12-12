using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;
using System.Text.RegularExpressions;

namespace BTCom
{
    public class BluetoothConnection : SerialPort
    {
        // Delimeter bytes
        const byte START_BYTE = 0x2;
        const byte END_BYTE = 0x3;

        // Type bytes to recieve
        const byte TYPE_UPDATE_COLOR = 0x41;
        const byte TYPE_SAVE_COLOR = 0x42;
        const byte TYPE_REPORT_OBSTACLE = 0x43;
        const byte TYPE_UPDATE_STATUS = 0x45;
        
        //Type bytes to send
        const byte TYPE_FETCHED_COLOR = 0x47;

        public const byte TYPE_DELIVER_PALLET = 0x44;
        public const byte TYPE_FETCH_PALLET = 0x46;
        public const byte TYPE_NAVIGATE_TO = 0x4E;

        // NXT statuses 
        public const char STATUS_IDLE = 'I';
        public const char STATUS_BUSY = 'B';
        public const char STATUS_OBSTACLE = 'O';
        public const char STATUS_ERROR = 'E';
        public const char STATUS_UNKNOWN = 'U';

        // Joblist queue of package type and data
        private Dictionary<int, Job> JobList = Database.Instance.Data.Jobs;
        
        // The current job 
        public static Job CurrentJob = null;

        private Forklift forklift = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

        // Constructor
        public BluetoothConnection(string portName) : base(portName)
        {
            // If the connection is not established try to connect.
            ConsoleHandler.AddMessage(MessageType.BLUETOOTH, "Attempting to connect to " + ConsoleHandler.DNS + "...");
            while (!this.IsOpen)
            {
                try
                {
                    this.Open(); // Try to open the connection 
                    Program.HasConnection = true;
                }
                catch
                {
                    // Catched by retrying after sleep
                    ConsoleHandler.AddMessage(MessageType.BLUETOOTH, "Connection failed. Trying again...");
                }
            }
            
            // Tell the user that the connection was established
            ConsoleHandler.AddMessage(MessageType.BLUETOOTH, "Connection established.");

            this.ReadTimeout = 10000; // Wait 10 sec before timeout on readbuffer
            this.WriteTimeout = 10000; // Wait 10 sec before timeout on writebuffer
        }

        // Consumes all packages on the buffer one at the time
        public void ConsumePackages()
        {
            byte[] btInputBuffer = ReadBTBuffer(); // The input on the buffer
            ConsumePackages(btInputBuffer); // Consume the buffer
        }

        // Consumes all packages on the buffer one at the time
        private void ConsumePackages(byte[] btInputBuffer)
        {
            int endIndexOfPackage = 0; // The index where the package ends

            // Run through the array of bytes on the buffer
            for (int i = 0; i < btInputBuffer.Length; i++)
            {
                if (btInputBuffer[i] == END_BYTE)
                {
                    byte[] package = new byte[i + 1]; // Allocate space for a pacakge
                    Array.Copy(btInputBuffer, package, i + 1); // Copies a package out
                    ConsumePackage(package); // Consume a package
                    endIndexOfPackage = i; // Sets the lates index to be the one from where we want to 
                    break;
                }
            }

            // If the package was not the last
            if (btInputBuffer.Length > endIndexOfPackage + 1)
            {
                // Allocate space for a new array to be splitted
                int newInputLength = btInputBuffer.Length - (endIndexOfPackage + 1);
                byte[] newBtInputBuffer = new byte[newInputLength];

                // Copies the rest of the array 
                Array.Copy(btInputBuffer, endIndexOfPackage + 1, newBtInputBuffer, 0, newInputLength);

                // Recursively splits this array
                ConsumePackages(newBtInputBuffer);
            }

        }

        // Sends a package with a given type
        private void SendPackageBT(byte packageType, byte[] package_data)
        {
            // Calculate the size of package_data
            int packageDataSize = package_data.Length;

            // Calculate the total package size, assuming the package data size and
            // start, end, and type bytes.
            int packageTotalSize = (packageDataSize + 3);

            // Check whether total data size is bigger than 256 bytes.
            if (packageTotalSize > 256)
            {
                // TODO: Send an error code for too big data array
                return;
            }

            // Make an array that is as large as the total data size of the package.
            byte[] packageToSend = new byte[packageTotalSize];

            // Putting the start, end, and type byte in the array.
            packageToSend[0] = START_BYTE;
            packageToSend[1] = packageType;
            packageToSend[packageTotalSize - 1] = END_BYTE;

            // Fill the package data into the array at the correct array index
            for (int i = 0; i < packageDataSize; i++)
            {
                packageToSend[i + 2] = (byte)package_data[i];
            }

            // Send the request over BT
            this.Write(packageToSend, 0, packageTotalSize);

            // Wait to ensure that continous requests is possible
            System.Threading.Thread.Sleep(1);

            return;
        }

        // Reads the bytes on the buffer
        private byte[] ReadBTBuffer()
        {
            // The amount of bytes that is on the readbuffer
            int bytesToRead = this.BytesToRead;

            // Allocate array with enough space according to the amount of bytes
            byte[] input = new byte[bytesToRead];

            // Read from the buffer
            this.Read(input, 0, bytesToRead);

            // Return the byte array from the buffer
            return input;
        }

        // Consumes one package 
        private void ConsumePackage(byte[] package)
        {
            // Allocates data from the package
            byte[] data = new byte[package.Length - 3];

            // Finds the type of pakcage
            byte byteType = package[1];

            // Fills the data array with the corresponding data
            for (int i = 2, j = 0; i < package.Length - 1; i++, j++)
            {
                data[j] = package[i];
            }

            // Convert the data to a string 
            string dataString = Encoding.UTF8.GetString(data, 0, data.Length);

            // Find the correct method to consume the package
            switch (byteType)
            {
                case TYPE_UPDATE_COLOR:
                    // Tell the user that the color is being fetched 
                    // dataString contains the id of the color
                    ConsoleHandler.AddMessage(MessageType.BLUETOOTH, "Sending color #" + dataString + "...");

                    // Get the color from the database
                    Color requestedColor = Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Identifier == int.Parse(dataString)).Value;

                    // Convert the RGB-values of the color to byte array
                    byte[] returnData = requestedColor.ToRGBBytes();

                    // Send the package back to the NXT
                    SendPackageBT(TYPE_FETCHED_COLOR, returnData);
                    
                    break;

                case TYPE_SAVE_COLOR:
                      
                    // Split the data into to values that needs to be saved
                    int colorId = int.Parse(dataString.Substring(0,3)); // Use the first char as color identifer
                    int red = int.Parse(dataString.Substring(3,3)); // Use the first char as red value
                    int green = int.Parse(dataString.Substring(6,3)); // Use the first char as green value
                    int blue = int.Parse(dataString.Substring(9,3)); // Use the first char as blue value

                    // Tell the user that the color is being saved 
                    ConsoleHandler.AddMessage(MessageType.BLUETOOTH, "Received color #" + colorId + ". (" + red + ", " + green + ", " + blue + ")");

                    // Find the old color
                    Color color = Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Identifier == colorId).Value;

                    // Update the values
                    color.Red = red;
                    color.Green = green;
                    color.Blue = blue;

                    // Save the updated color
                    Database.Instance.Data.AddColor(color);
                    break;

                case TYPE_REPORT_OBSTACLE:

                    ConsoleHandler.AddMessage(MessageType.ERROR, "Obstacle encountered!");
                    ConsoleHandler.AddMessage(MessageType.REGULAR, "Calculating alternative path...");

                    int directionsIndex = int.Parse(dataString);

                    if (!(CurrentJob is TurnJob) && !(CurrentJob is DebugJob))
                    {
                        if (CurrentJob != null)
                        {
                            Path currentPath = CurrentJob.GetPath();

                            String directions = new ForkliftPath(currentPath, forklift.RearNode).getDirections();

                            if (CurrentJob is PalletJob)
                            {
                                PalletJob pj = (PalletJob) CurrentJob;

                                if (pj.Type == PalletJobType.fetch)
                                {
                                    directions = directions.Insert(0, "TUND");
                                }
                                else if (pj.Type == PalletJobType.deliver)
                                {
                                    directions = directions.Insert(0, "TUBDN");
                                }
                            }

                            int max = directions.Length;
                            int i = directionsIndex;

                            if (directions[directions.Length - 1] == 'L' || directions[directions.Length - 1] == 'R')
                            {
                                max--;
                            }

                            double nodeRearIndex = (max - 1 - i)/2;

                            if (Math.Abs(nodeRearIndex % 1 - 0.5) < 0.1)
                            {
                                Commands.PrintError("PALL-E: Something went wront, please fix it");
                                throw new Exception();
                            }
                            else
                            {
                                Node newRearNode = null;
                                Node newFrontNode = null;
                                int roundedIndex = (int) nodeRearIndex;

                                if (roundedIndex == 0)
                                {
                                    newFrontNode = forklift.FrontNode;
                                    newRearNode = forklift.RearNode;
                                }
                                else
                                {
                                    newFrontNode = currentPath.Nodes[roundedIndex];
                                    newRearNode = currentPath.Nodes[roundedIndex - 1];
                                }

                                // Update the visitied count on edges
                                Node previousNode = null;

                                foreach (Node n in currentPath.Nodes)
                                {
                                    if (previousNode == null)
                                    {
                                        previousNode = n;
                                        continue;
                                    }

                                    // Find edge to the previous node and increment the visitied count
                                    KeyValuePair<Node, Edge> nodeEdgePair = previousNode.Neighbours.Single(x => x.Key != null && x.Key.Equals(n));
                                    nodeEdgePair.Value.Visited++;

                                    if (n.Equals(newFrontNode))
                                    {
                                        break;
                                    }

                                    previousNode = n;
                                }

                                // Update forklift nodes (Must be reversed because of PALL-E behaviour)
                                forklift.UpdateNodes(newRearNode, newFrontNode);

                                // Update edge the NXT is standing on
                                Database.Instance.Data.Graphs.FirstOrDefault().Value.BlockEdge(newFrontNode, newRearNode);
                            }
                        }
                        else
                        {
                            Commands.PrintError("No current job - Continuing...");
                        }
                    }
                    else if(CurrentJob is TurnJob)
                    {
                        Commands.PrintError("Already performing turn-job, ignoreing...");
                    }
                    else if (CurrentJob is DebugJob)
                    {
                        // TODO: Change to notification
                        Commands.PrintError("Performing debug-job, ignoreing...");
                    }
                    
                    break;

                // Check if the NXT updated its status
                case TYPE_UPDATE_STATUS:
                {
                    if (dataString[0] != GetStatusByte(forklift))
                    {
                        // Tell the user what the status the NXT updated to
                        ConsoleHandler.AddMessage(MessageType.BLUETOOTH, "Received " + ConsoleHandler.DNS + "-status: " + dataString);
                    }

                    // Check what status the NXT told us
                    switch (dataString[0])
                    {
                        // The NXT was idle
                        case STATUS_IDLE:

                            // Check if the nxt just completed a job
                            if (forklift.Status == Status.BUSY && CurrentJob != null)
                            {
                                Path p = CurrentJob.GetPath();

                                // Update the visited count on traversed edges
                                Node previousNode = null;

                                foreach (Node n in p.Nodes)
                                {
                                    if (previousNode == null)
                                    {
                                        previousNode = n;
                                        continue;
                                    }

                                    // Find edge to the previous node and increment the visitied count
                                    KeyValuePair<Node, Edge> nodeEdgePair = previousNode.Neighbours.Single(x => x.Key != null && x.Key.Equals(n));
                                    nodeEdgePair.Value.Visited++;

                                    previousNode = n;
                                }

                                // Update the position of the forklift
                                if (p.Nodes.Count >= 2)
                                {
                                    Node frontNode = p.Nodes.ElementAt(p.Nodes.Count - 1);
                                    Node rearNode = p.Nodes.ElementAt(p.Nodes.Count - 2);

                                    Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;
                                    f.UpdateNodes(frontNode, rearNode);
                                }

                                if (!(CurrentJob is TurnJob))
                                {
                                    Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

                                    // Unblock all edges
                                    for (int i = g.BlockedEdges.Count - 1; i >= 0; i--)
                                    {
                                        Tuple<Node, Node> edge = g.BlockedEdges[i];
                                        g.UnblockEdge(edge.Item1, edge.Item2);
                                    }

                                    // Decay the graph
                                    g.Decay(0.90);
                                }

                                if (CurrentJob is PalletJob)
                                {
                                    PalletJob job = (PalletJob) CurrentJob;

                                    // Check if the forklift just finished a deliver pallet job
                                    if (job.Type == PalletJobType.deliver)
                                    {
                                        Pallet pallet = forklift.Payload;

                                        // Update the location of the pallet
                                        pallet.Location = p.Nodes.Last();

                                        // Update the payload of the forklift
                                        forklift.Payload = null;
                                    }
                                    else if (job.Type == PalletJobType.fetch)
                                    {
                                        Node n = p.Nodes.Last();

                                        // Update the payload (Which will also update the location of the pallet)
                                        forklift.Payload = n.Pallet;
                                    }

                                    // Swap front and rear node
                                    forklift.UpdateNodes(forklift.RearNode, forklift.FrontNode);
                                }

                                CurrentJob = null;
                            }

                            // Check if there is any jobs to be performed
                            if (JobList.Count > 0)
                            {
                                Job nextJob = null;

                                while ((nextJob == null || nextJob.GetPath() != null) && JobList.Count > 0)
                                {
                                    // Get the next job (job with lowest id)
                                    nextJob = JobList.Aggregate((l, r) => l.Key < r.Key ? l : r).Value;

                                    try
                                    {
                                        nextJob.GetPath();
                                        break;
                                    }
                                    catch (PathException e)
                                    {
                                        Commands.PrintError("Remove job #" + nextJob.ID() + ": '" + e.Message + "'");
                                    }
                                    catch (JobException e)
                                    {
                                        Commands.PrintError("Remove job #" + nextJob.ID() + ": '" + e.Message + "'");
                                    }

                                    Database.Instance.Data.RemoveJob(nextJob);
                                    nextJob = null;
                                }

                                // We removed all the jobs, nothing left to do!
                                if (JobList.Count < 1)
                                {
                                    break;
                                }

                                try
                                {
                                    // Test if the path can be generated
                                    nextJob.GetPath();

                                    // Tell the user what job was sent
                                    Commands.PrintSuccess("Sending Job #" + nextJob.ID() + " to PALL-E. " + (JobList.Count - 1) + " jobs left");

                                    // Send the job to the NXT
                                    SendPackageBT(nextJob.GetJobTypeBytes(), nextJob.GetBytes());
                                }
                                catch (JobException e)
                                {
                                    Commands.PrintError("Job #" + nextJob.ID() + " cancelled: '" + e.Message + "'");
                                    Database.Instance.Data.RemoveJob(nextJob);
                                }
                            }

                            // Update the internal status
                            forklift.Status = Status.IDLE;
                            break;

                        // The NXT was busy
                        case STATUS_BUSY:

                            // If the NXT was just idle, you know you have given it a job
                            if (forklift.Status == Status.IDLE)
                            {
                                // Debug jobs has highe priority
                                if(JobList.Count > 0)
                                {
                                    // Remove the job that is being executed currently
                                    CurrentJob = JobList.Aggregate((l, r) => l.Key < r.Key ? l : r).Value;
                                    Database.Instance.Data.Jobs.Remove(CurrentJob.Identifier);
                                }   
                            }

                            // Update the internal status
                            forklift.Status = Status.BUSY;
                            break;

                        case STATUS_OBSTACLE:
                            if (CurrentJob != null)
                            {
                                if (!(CurrentJob is DebugJob))
                                {

                                    if (!(CurrentJob is TurnJob))
                                    {
                                        Database.Instance.Data.AddJob(CurrentJob);
                                        CurrentJob = new TurnJob(Database.Instance.Data.Jobs.Keys.Min() - 1);

                                        Commands.PrintSuccess("Sending turn-job to PALL-E...");
                                    }
                                    else
                                    {
                                        ConsoleHandler.AddMessage(MessageType.REGULAR, "Resending turn-job to " + ConsoleHandler.DNS);
                                    }

                                    // Send turn job
                                    SendPackageBT(CurrentJob.GetJobTypeBytes(), CurrentJob.GetBytes());
                                }
                                else
                                {
                                    // Send empty package (so that obstacle status will be avoided
                                    DebugJob dj = new DebugJob(-1, "");
                                    SendPackageBT(dj.GetJobTypeBytes(), dj.GetBytes());
                                }
                            }
                            else
                            {
                                // throw new Exception("No current jobs");
                            }

                            // The status should not be updated if the current job is a debugjob
                            if (!(CurrentJob is DebugJob))
                            {
                                // Update the internal status
                                forklift.Status = Status.OBSTACLE;
                            }

                            break;

                        // The NXT encoutered an error
                        case STATUS_ERROR:
                            // Update the internal status
                            forklift.Status = Status.ERROR;
                            
                            // Tell the user that the NXT encountered an error
                            ConsoleHandler.AddMessage(MessageType.ERROR, ConsoleHandler.DNS + " reported an error");
                            break;

                        default:
                            forklift.Status = Status.UNKNOWN;
                            break;
                    }
                    break;
                }
            }
            return;
        }

        // Used to convert status to byte
        public char GetStatusByte(Forklift f)
        {
            switch (f.Status)
            {
                case Status.IDLE:
                    return STATUS_IDLE;
                case Status.BUSY:
                    return STATUS_BUSY;
                case Status.ERROR:
                    return STATUS_ERROR;
                case Status.OBSTACLE:
                    return STATUS_OBSTACLE;
                default :
                    return STATUS_UNKNOWN;
            }
        }
    }
}
