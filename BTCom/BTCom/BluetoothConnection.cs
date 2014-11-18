using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;

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

        const byte TYPE_DELIVER_PALLET = 0x44;
        const byte TYPE_FETCH_PALLET = 0x46;
        const byte TYPE_NAVIGATE_TO = 0x4E;

        // NXT statuses 
        char NXTStatus = '0';
        const char IDLE = 'I';
        const char BUSY = 'B';
        const char ERROR = 'E';

        // Joblist queue of package type and data
        private List<Job> JobList = new List<Job>();

        // The current job 
        private Job CurrentJob;

        // Constructor
        public BluetoothConnection(string portName)
            : base(portName)
        {
            // If the connection is not established try to connect.
            Console.WriteLine("Attempting to connect to NXT via BT.");
            while (!this.IsOpen)
            {
                try
                {
                    this.Open(); // Try to open the connection    
                }
                catch
                {
                    // Catched by retrying after sleep
                    Console.WriteLine("Connection failed. Attempting to connect to NXT via BT.");
                }
            }
            
            // Tell the user that the connection was established
            Console.WriteLine("Connection established.");

            this.ReadTimeout = 10000; // Wait 10 sec before timeout on readbuffer
            this.WriteTimeout = 10000; // Wait 10 sec before timeout on writebuffer
        }

        // Create a job based on string input (console input)
        public void CreateJob(string input)
        {   
            // Split the input
            List<string> inputSplit = input.Split(' ').ToList();

            inputSplit.RemoveAll(s => s == "");

            string cmd = "";
            if (inputSplit.Count > 0)
            {
                cmd = inputSplit[0];
            }

            // Check if the input is 
            switch (cmd)
            {
                case "joblist":
                    if (inputSplit.Count == 1)
                    {
                        if (JobList.Count > 0 || CurrentJob != null)
                        {
                            Console.WriteLine("------------------------------ Joblist of PALL-E ------------------------------");
                            if (CurrentJob != null)
                            {
                                Console.WriteLine("Current job: " + CurrentJob.ToString());
                            }
                        }
                        else
                        {
                            Console.WriteLine("The joblist of PALL-E is empty");
                        }

                        int c = 0;
                        foreach(Job job in JobList)
                        {   
                            Console.WriteLine("Job #" + c + ": " + job.ToString());
                            c++;
                        }
                    }
                    else if (inputSplit.Count > 1)
                    {
                        if (inputSplit[1] == "remove")
                        {
                            if (inputSplit.Count > 2)
                            {
                                int index = -1;
                                try
                                {
                                    index = int.Parse(inputSplit[2]);
                                }
                                catch (Exception)
                                {
                                    Console.WriteLine("Invalid argument! Correct use => joblist [\"remove\"] [Index]");
                                }

                                if (index != -1)
                                {
                                    if (index < JobList.Count)
                                    {
                                        JobList.RemoveAt(index);
                                        Console.WriteLine("Removed job at index " + index + ".");
                                    }
                                    else
                                    {
                                        Console.WriteLine("Cant remove the job on that index. There are only " + JobList.Count + " jobs in the list.");
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("Invalid use! Correct use => joblist [\"remove\"] [Index]");
                            }
                        }
                        else if(inputSplit[1] == "clear")
                        {
                            JobList.RemoveRange(0, JobList.Count);
                            Console.WriteLine("The joblist for PALL-E was cleared.");
                        }
                        else
                        {
                            Console.WriteLine("Correct use => joblist [\"remove\" Index]/[\"clear\"]");
                        }
                    }
                    break;

                case "navigate" :
                case "deliver" :
                case "fetch" :
                    if(inputSplit.Count == 2)
                    {
                        Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

                        Node to = null;

                        try
                        {
                            to = g.getNode(inputSplit[1]); // First argument    
                        }
                        catch (NodeException e)
                        {
                            Console.WriteLine("Error when finding node:");
                            Console.WriteLine(e.Message);
                            break;
                        }

                        byte type = 0;

                        switch (inputSplit[0])
                        {
                            case "navigate":
                                type = TYPE_NAVIGATE_TO;
                                break;
                            case "deliver":
                                type = TYPE_DELIVER_PALLET;
                                break;
                            case "fetch":
                                type = TYPE_FETCH_PALLET;
                                break;
                        }

                        JobList.Add(new Job(type, to));
                    }
                    else
                    {
                        Console.WriteLine("Wrong use. Correct use => \"navigate\"/\"deliver\"/\"fetch\"/ {DestinationNode}");
                    }
                    break;

                case "help" :
                    if (inputSplit.Count > 1)
                    {
                        switch (inputSplit[1])
                        {
                            case "navigate":
                            case "deliver":
                            case "fetch":
                                Console.WriteLine("Wrong use. Correct use => \"navigate\"/\"deliver\"/\"fetch\"/ {DestinationNode}");
                                break;
                            case "joblist":
                                Console.WriteLine("Correct use => joblist [\"remove\" Index]/[\"clear\"]");
                                break;
                            default :
                                Console.WriteLine("Invalid parameter for help. Correct use => help [Command]");
                                break;
                        }
                    }
                    else if (inputSplit.Count == 1)
                    {
                        Console.WriteLine("----------------------------- Commands for PALL-E -----------------------------");
                        Console.WriteLine("deliver  \t: PALL-E is sent to deliver a pallet at a given place.");
                        Console.WriteLine("fetch    \t: PALL-E is sent to fetch a pallet at a given place.");
                        Console.WriteLine("joblist  \t: Print the joblist for PALL-E.");
                        Console.WriteLine("navigate \t: Navigates PALL-E to specific place.");
                    }
                    else
                    {
                        Console.WriteLine("Invalid parameter for help. Correct use => help [Command]");
                    }
                    break;
                default :
                    Console.WriteLine("Invalid input, type help for all commands.");
                    break;
            }

            Console.WriteLine();
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
                    Console.WriteLine("Fetching color with ID: \'" + dataString + "\'");

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
                    Console.WriteLine("Saving the color with ID: \'" + colorId + "\' with values {" + red + ", " + green + ", " + blue + "}");

                    // Find the old color
                    Color color =
                        Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Identifier == colorId).Value;

                    // Update the values
                    color.Red = red;
                    color.Green = green;
                    color.Blue = blue;

                    // Save the updated color
                    Database.Instance.Data.AddColor(color);
;                    break;

                case TYPE_REPORT_OBSTACLE:
                    // TODO
                    break;

                // Check if the NXT updated its status
                case TYPE_UPDATE_STATUS:
                {
                    if (dataString[0] != NXTStatus)
                    {
                        // Tell the user what the status the NXT updated to
                        Console.WriteLine("NXT-Status: " + dataString);
                    }

                    // Check what status the NXT told us
                    switch (dataString[0])
                    {
                        // The NXT was idle
                        case IDLE:

                            // Check if the nxt just completed a job
                            if (NXTStatus == BUSY)
                            {
                                //TODO: Update the position of PALL-E
                                Path p = CurrentJob.GetPath();

                                Node frontNode = p.Nodes.ElementAt(p.Nodes.Count - 1);
                                Node rearNode = p.Nodes.ElementAt(p.Nodes.Count - 2);

                                Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;
                                f.UpdateNodes(frontNode, rearNode);
                            }


                            // Check if there is any jobs to be performed
                            if (JobList.Count > 0)
                            {
                                // Peek in the queue
                                Job nextJob = JobList[0];

                                // Tell the user what job was sent
                                Console.WriteLine("Sending Job -> NXT: " + nextJob.ToString() + ". " + (JobList.Count-1) + " jobs left in the JobList");

                                // Send the job to the NXT
                                SendPackageBT(nextJob.Type, nextJob.GetByes());
                            }
                            // Update the internal status
                            NXTStatus = IDLE;
                            break;

                        // The NXT was busy
                        case BUSY:

                            // If the NXT was just idle, you know you have given it a job
                            if (NXTStatus == IDLE)
                            {
                                // Remove the job that is being executed currently
                                CurrentJob = JobList[0];
                                JobList.RemoveAt(0);
                            }

                            // Update the internal status
                            NXTStatus = BUSY;
                            break;

                        // The NXT encoutered an error
                        case ERROR:
                            // Tell the suer that the NXT encountered an error
                            Console.WriteLine("The NXT has encountered an error!");
                            break;
                    }
                    break;
                }
            }
            return;
        }
    }
}
