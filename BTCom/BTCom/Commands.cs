using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using PALL_E_Terminal.Exceptions;

namespace PALL_E_Terminal
{
    public static class Commands
    {
        private const String HELP = "help";

        private const String SUBCOMMAND_ADD = "add";
        private const String SUBCOMMAND_REMOVE = "remove";
        private const String SUBCOMMAND_CLEAR = "clear";

        private const String COMMAND_STATUS = "status";
        private const String COMMAND_JOBLIST = "joblist";
        private const String COMMAND_DELIVER = "deliver";
        private const String COMMAND_DEBUG = "debug";
        private const String COMMAND_FETCH = "fetch";
        private const String COMMAND_NAVIGATE = "navigate";
        private const String COMMAND_POSITION = "position";
        private const String COMMAND_CURRENTJOB = "currentjob";
        private const String COMMAND_PALLETLIST = "palletlist";
        private const String COMMAND_PAYLOAD = "payload";
        private const String COMMAND_NODE = "node";
        private const String COMMAND_UNBLOCK = "unblock";
        private const String COMMAND_CLEAR = "clear";
        private const String COMMAND_RELOAD = "reload";
        private const String COMMAND_SAVE = "save";
        private const String COMMAND_EXIT = "exit";

        private const String COMMAND_ARGUMENT_CLEAR = "clear";

        public static void PrintError(string s)
        {
            ConsoleHandler.AddMessage(MessageType.ERROR, "Error: " + s);
            Console.ResetColor();
        }

        public static void PrintSuccess(string s)
        {
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "Success: " + s);
        }

        private static void PrintIncorrectUse()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "Incorrect use:");
        }

        private static void PrintInvalidCommand()
        {
            PrintError("Invalid command. Type \"help\" for help.");
        }

        public static void Execute()
        {
            while (true)
            {
                Execute(Console.ReadLine());
                ConsoleHandler.ClearCommand();
                Thread.Yield(); // Other threads should be able to run after each command
            }
        }

        private static void Execute(string command)
        {
            command = command.ToLower();

            // No command provided, user must need help
            if (String.IsNullOrEmpty(command))
            {
                PrintInvalidCommand();
                return;
            }

            // Split the string into command and arguments
            List<String> commandAndArguments = command.Split('-').ToList();

            // No command provided, user must need help
            if (String.IsNullOrEmpty(commandAndArguments[0]))
            {
                PrintInvalidCommand();
                return;
            }

            // Split the command at every space
            List<string> commandSplit = commandAndArguments[0].Split(' ').ToList();

            // Remove empty commands (occours when multiple spaces are entered)
            commandSplit.RemoveAll(s => s == "");

            // Save the first part of the command
            string commandIdentifier = commandSplit[0];

            // Remove the first part of the command
            commandSplit.RemoveAt(0);

            int arguments = commandSplit.Count;

            commandSplit.RemoveAll(s => s == "");

            // Check if the command argument is "clear"
            List<String> argumentList = commandAndArguments.Skip(1).ToList();

            // Trim all command arguments
            for (int i = 0; i < argumentList.Count; i++)
            {
                argumentList[i] = argumentList[i].Trim();
            }

            // Check if an argument is to clear the display
            if (argumentList.Contains(COMMAND_ARGUMENT_CLEAR))
            {
                argumentList.RemoveAll(x => x == COMMAND_ARGUMENT_CLEAR);
                Clear();
            }

            // Check if there are unprocessed arguments
            if (argumentList.Count > 0)
            {
                String error = "Unknown command arguments: ";

                foreach (string s in argumentList)
                {
                    if (s != argumentList.FirstOrDefault())
                    {
                        error += ", ";
                    }

                    error += "'" + s + "'";
                }

                error += "\n";

                ConsoleHandler.AddMessage(MessageType.ERROR, error);

                return;
            }

            // Check if the command is "help"
            if (commandIdentifier == HELP)
            {
                if (arguments == 0)
                {
                    Help();    
                }
                else
                {
                    PrintIncorrectUse();
                    HelpHelp();
                }
            } 
            else if (commandIdentifier == COMMAND_STATUS)
            {
                if (arguments == 0)
                {
                    Status();
                }
                else
                {
                    PrintIncorrectUse();
                    StatusHelp();
                }
            }
            // Check if the command is "joblist"
            else if (commandIdentifier == COMMAND_JOBLIST)
            {
                if (arguments == 0)
                {
                    Joblist();
                }
                else if (arguments == 1)
                {
                    Joblist(commandSplit[0]);
                }
                else if (arguments == 2)
                {
                    Joblist(commandSplit[0], commandSplit[1]);
                }
                else
                {
                    PrintIncorrectUse();
                    JoblistHelp();
                }
            }
            else if (commandIdentifier == COMMAND_CURRENTJOB)
            {
                if (arguments == 0)
                {
                    CurrentJob();
                }
                else
                {
                    PrintIncorrectUse();
                    CurrentJobHelp();
                }
            }
            // Check if the command is "deliver", "fetch" or "navigate"
            else if (commandIdentifier == COMMAND_FETCH)
            {
                if (arguments == 2)
                {
                    if (commandSplit[0] == "node")
                    {
                        FetchNode(commandSplit[1]);    
                    }
                    else if(commandSplit[0] == "pallet")
                    {
                        FetchPallet(commandSplit[1]);
                    }
                    else
                    {
                        PrintIncorrectUse();
                        FetchNodeHelp();
                        FetchPalletHelp();
                    }
                }
                else
                {
                    PrintIncorrectUse();
                    FetchNodeHelp();
                    FetchPalletHelp();
                }
            }
            else if (commandIdentifier == COMMAND_DELIVER || 
                     commandIdentifier == COMMAND_NAVIGATE)
            {
                if (arguments == 1)
                {
                    DeliverNavigate(commandIdentifier, commandSplit[0]);
                }
                else
                {
                    PrintIncorrectUse();
                    DeliverNavigateHelp();
                }
            }
            else if (commandIdentifier == COMMAND_DEBUG)
            {
                if (arguments == 1)
                {
                    Debug(commandSplit[0]);
                }
            }
            else if (commandIdentifier == COMMAND_POSITION)
            {
                if (arguments == 0)
                {
                    Position();
                }
                else if (arguments == 2)
                {
                    Position(commandSplit[0], commandSplit[1]);
                }
                else
                {
                    PrintIncorrectUse();
                    PositionHelp();
                }
            }
            else if (commandIdentifier == COMMAND_CLEAR)
            {
                if (arguments == 0)
                {
                    Clear();
                }
                else
                {
                    PrintIncorrectUse();
                    ClearHelp();
                }
            }
            else if (commandIdentifier == COMMAND_PALLETLIST)
            {
                if (arguments == 0)
                {
                    Palletlist();
                }
                else if (arguments == 1)
                {
                    Palletlist(commandSplit[0]);
                }
                else if (arguments == 2)
                {
                    Palletlist(commandSplit[0], commandSplit[1]);
                }
                else if (arguments == 3)
                {
                    Palletlist(commandSplit[0], commandSplit[1], commandSplit[2]);
                }
                else
                {
                    PrintIncorrectUse();
                    PalletlistHelp();
                }
            }
            else if (commandIdentifier == COMMAND_PAYLOAD)
            {
                if (arguments == 0)
                {
                    Payload();
                }
                else if (arguments == 1)
                {
                    Payload(commandSplit[0]);
                }
                else
                {
                    PrintIncorrectUse();
                    PayloadHelp();
                }
            }
            else if (commandIdentifier == COMMAND_NODE)
            {
                if (arguments == 1)
                {
                    Node(commandSplit[0]);
                }
                else
                {
                    NodeHelp();
                }
            }
            else if (commandIdentifier == COMMAND_UNBLOCK)
            {
                if (arguments == 2)
                {
                    Unblock(commandSplit[0], commandSplit[1]);
                }
                else if (arguments == 1)
                {
                    Unblock(commandSplit[0]);
                }
                else
                {
                    UnblockHelp();
                }
            }
            else if (commandIdentifier == COMMAND_RELOAD)
            {
                if (arguments == 0)
                {
                    Reload();
                }
                else
                {
                    PrintIncorrectUse();
                    ReloadHelp();
                }
            }
            else if (commandIdentifier == COMMAND_SAVE)
            {
                if (arguments == 0)
                {
                    Save();
                }
                else
                {
                    PrintIncorrectUse();
                    SaveHelp();
                }
            }
            else if (commandIdentifier == COMMAND_EXIT)
            {
                if (arguments == 0)
                {
                    Exit();
                }
                else
                {
                    PrintIncorrectUse();
                    ExitHelp();
                }
            }
            // Some unknown command, show the help
            else
            {
                PrintInvalidCommand();
            }

            ConsoleHandler.ClearCommand();
        }

        public static void Help()
        {
            PositionHelp();
            PayloadHelp();
            JoblistHelp();
            PalletlistHelp();
            DeliverNavigateHelp();
            FetchNodeHelp();
            FetchPalletHelp();
            DebugHelp();
            NodeHelp();
            UnblockHelp();
            HelpHelp();
            ClearHelp();
            SaveHelp();
            ExitHelp();
        }

        public static void HelpHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"help\"");
        }

        public static void Status()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            ConsoleHandler.AddMessage(MessageType.REGULAR, "Status: " + f.Status);
        }

        public static void StatusHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"status\"");
        }

        public static void Joblist()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "Current joblist:");

            List<int> IDlist = Database.Instance.Data.Jobs.Keys.ToList();
            IDlist.Sort();

            int c = 0;
            foreach (int key in IDlist)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, Database.Instance.Data.Jobs[key].ToString());
                c++;
            }

            if (c == 0)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "(Empty)");
            }
            
        }

        public static void Joblist(string subCommand)
        {
            if (subCommand.ToLower() == SUBCOMMAND_CLEAR)
            {
                // Remove all jobs
                Database.Instance.Data.Jobs = new Dictionary<int, Job>();

                PrintSuccess("Joblist was cleared");
            }
            else
            {
                PrintIncorrectUse();
                JoblistHelp();
            }
        }

        public static void Joblist(string subCommand, string identifier)
        {
            if (subCommand.ToLower() == SUBCOMMAND_REMOVE)
            {
                int i;

                // Test if the second argument is a number
                if (int.TryParse(identifier, out i))
                {
                    Job j;

                    // Test if a job with that identifier could be found 
                    if (Database.Instance.Data.Jobs.TryGetValue(i, out j))
                    {
                        if (Database.Instance.Data.RemoveJob(j))
                        {
                            PrintSuccess("Job removed");
                        }
                        else
                        {
                            PrintError("Could not find job");
                        }
                    }
                    else
                    {
                        PrintError("Could not find job with identifier '" + i + "'");
                    }
                }
                else
                {
                    PrintError("{ID} is not a correct identifier");
                    JoblistHelp();
                }
            }
            else
            {
                PrintIncorrectUse();
                JoblistHelp();
            }
        }

        public static void JoblistHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"joblist\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"joblist\" \"clear\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"joblist\" \"remove\" {[0-9]+}");
        }

        public static void FetchNode(string node)
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            Node destination = null;

            try
            {
                destination = g.getNode(node);
            }
            catch (NodeException e)
            {
                PrintError(e.Message);
                return;
            }

            try
            {
                if (!destination.IsPalletNode)
                {
                    PrintError("Node '" + destination.Name + "' is not a pallet node");
                    return;
                }

                Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), destination, PalletJobType.fetch));
                PrintSuccess("Job added");
            }
            catch (Exception e)
            {
                PrintError(e.Message);
            }
        }

        public static void FetchNodeHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"fetch\" \"node\" {node}");
        }

        public static void DeliverNavigate(string type, string node)
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            Node destination = null;

            try
            {
                destination = g.getNode(node);
            }
            catch (NodeException e)
            {
                PrintError(e.Message);
                return;
            }

            if (type == COMMAND_DELIVER)
            {
                try
                {
                    if (!destination.IsPalletNode)
                    {
                        PrintError("Node '" + destination.Name + "' is not a pallet node");
                        return;
                    }

                    Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), destination, PalletJobType.deliver));
                    PrintSuccess("Job added");
                }
                catch (Exception e)
                {
                    PrintError(e.Message);
                }
            }
            else if (type == COMMAND_NAVIGATE)
            {
                try
                {
                    Database.Instance.Data.AddJob(new NavigateJob(Database.Instance.Data.GetNewJobIdentifier(), destination));
                    PrintSuccess("Job added");
                }
                catch (Exception e)
                {
                    PrintError(e.Message);
                }
            }
            else
            {
                PrintIncorrectUse();
                FetchNodeHelp();
            }
        }

        public static void DeliverNavigateHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"deliver\" {node}");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"navigate\" {node}");
        }

        public static void FetchPallet(string palletName)
        {
            Pallet pallet = null;

            foreach (KeyValuePair<int, Pallet> palletPair in Database.Instance.Data.Pallets)
            {
                if (String.Equals(palletPair.Value.Name, palletName, StringComparison.CurrentCultureIgnoreCase))
                {
                    pallet = palletPair.Value;
                    break;
                }
            }

            if (pallet == null)
            {
                PrintError("Could not find pallet with name '" + palletName + "'");
                return;
            }

            try
            {
                Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), pallet, PalletJobType.fetch));
                PrintSuccess("Job added");
            }
            catch (Exception e)
            {
                PrintError(e.Message);
            }
        }

        public static void FetchPalletHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"fetch\" \"pallet\" {pallet}");
        }

        public static void Debug(string directions)
        {
            try
            {
                DebugJob dj = new DebugJob(Database.Instance.Data.GetNewJobIdentifier(), directions);
                Database.Instance.Data.AddJob(dj);
            }
            catch (FormatException e)
            {
                PrintError(e.Message);
            }
            
        }

        public static void DebugHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"debug\" {[BDLMRSTU]+}");
        }

        public static void Position()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            if (f.FrontNode == null || f.RearNode == null)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Position: Unknown");
            }
            else
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Front-node: " + f.FrontNode.Name);
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Rear-node:  " + f.RearNode.Name);   
            }
        }

        public static void Position(string frontNode, string rearNode)
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            Node front = null;
            Node rear = null;

            try
            {
                front = g.getNode(frontNode);
                rear = g.getNode(rearNode);
            }
            catch (NodeException e)
            {
                PrintError(e.Message);
                return;
            }

            try
            {
                f.UpdateNodes(front, rear);
            }
            catch (NodeException e)
            {
                PrintError(e.Message);
                return;
            }

            PrintSuccess("Position updated");
            Position();
        }

        public static void PositionHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"position\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"position\" <front node> <rear node>");
        }

        public static void CurrentJob()
        {
            if (BluetoothConnection.CurrentJob != null)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Current job: '" + BluetoothConnection.CurrentJob.ToString() + "'");
            }
            else
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "No current job");
            }
        }

        public static void CurrentJobHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"currentjob\"");
        }

        public static void Palletlist()
        {
            Dictionary<int, Pallet> pallets = Database.Instance.Data.Pallets;

            if (pallets.Count == 0)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "No pallets");
                return;
            }

            foreach (KeyValuePair<int, Pallet> pallet in pallets)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "#" + pallet.Key + " - " + pallet.Value.ToString());
            }
        }

        public static void Palletlist(string subcommand)
        {
            if (subcommand == COMMAND_CLEAR)
            {
                Database.Instance.Data.Pallets = new Dictionary<int, Pallet>();

                PrintSuccess("Pallet list cleared");
            }
            else
            {
                PrintIncorrectUse();
                PalletlistHelp();
            }
        }

        public static void Palletlist(string subcommand, string name, string location)
        {
            if (subcommand == SUBCOMMAND_ADD)
            {
                if (location == "forklift")
                {
                    Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

                    if (f.HasPallet)
                    {
                        PrintError("Forklift is already carrying pallet: '" + f.Payload.Name + "'");
                        return;
                    }

                    try
                    {
                        Pallet p = new Pallet(Database.Instance.Data.GetNewPalletIdentifier(), name.ToUpper(), null);
                        Database.Instance.Data.AddPallet(p);
                        f.Payload = p;

                        PrintSuccess("Pallet added.");
                    }
                    catch (PalletException e)
                    {
                        PrintError(e.Message);
                        return;
                    }
                }
                else
                {
                    Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

                    try
                    {
                        Node n = g.getNode(location);

                        if (!n.IsPalletNode)
                        {
                            PrintError("Node '" + n.Name + "' is not a pallet-node.");
                            return; 
                        }

                        if (n.HasPallet)
                        {
                            PrintError("Node '" + n.Name + "' is already containing a pallet.");
                            return; 
                        }

                        try
                        {
                            Pallet p = new Pallet(Database.Instance.Data.GetNewPalletIdentifier(), name.ToUpper(), n);
                            Database.Instance.Data.AddPallet(p);

                            PrintSuccess("Pallet added.");
                        }
                        catch (PalletException e)
                        {
                            PrintError(e.Message);
                            return;
                        }
                    }
                    catch (NodeException e)
                    {
                        PrintError(e.Message);
                        return;
                    }
                }
            }
            else
            {
                PrintIncorrectUse();
                PalletlistHelp();
            }
        }

        public static void Palletlist(string subcommand, string name)
        {
            if (subcommand == SUBCOMMAND_REMOVE)
            {
                foreach (KeyValuePair<int, Pallet> palletPair in Database.Instance.Data.Pallets)
                {
                    if (String.Equals(palletPair.Value.Name, name, StringComparison.CurrentCultureIgnoreCase))
                    {
                        Database.Instance.Data.RemovePallet(palletPair.Value);

                        Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

                        // Remove the payload fron the forklift
                        if (f.Payload.Equals(palletPair.Value))
                        {
                            f.Payload = null;
                        }

                        PrintSuccess("Pallet removed");
                        return;
                    }
                }

                PrintError("Could not find pallet with name '" + name + "'");
            }
            else
            {
                PrintIncorrectUse();
                PalletlistHelp();
            }
        }

        public static void PalletlistHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"palletlist\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"palletlist\" \"add\" <name> <node>");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"palletlist\" \"add\" <name> \"forklift\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"palletlist\" \"remove\" <name>");
        }

        public static void Payload()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            if (f.HasPallet)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Payload: '" + f.Payload.Name + "'");
            }
            else
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "No payload");
            }
        }

        public static void Payload(String newPayload)
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            if (f.HasPallet)
            {
                PrintError("Forklift is already carrying pallet: '" + f.Payload.Name + "'");
                return;
            }

            Pallet pallet = null;

            foreach (KeyValuePair<int, Pallet> palletPair in Database.Instance.Data.Pallets)
            {
                if (String.Equals(palletPair.Value.Name, newPayload, StringComparison.CurrentCultureIgnoreCase))
                {
                    pallet = palletPair.Value;
                    break;
                }
            }

            if (pallet == null)
            {
                PrintError("Could not find pallet with name '" + newPayload + "'");
                return;
            }

            Database.Instance.Data.Forklifts.FirstOrDefault().Value.Payload = pallet;
            PrintSuccess("Payload updated");
        }

        public static void PayloadHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"payload\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"payload\" <pallet>");
        }

        public static void Node(String node)
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            try
            {
                Node n = g.getNode(node);

                int neighbours = n.Neighbours.Count(x => x.Key != null);
                int blockedNeighbours = n.BlockedNeighbours.Count(x => x.Key != null);

                ConsoleHandler.AddMessage(MessageType.REGULAR, "Node '" + n.Name + "' has " + (neighbours + blockedNeighbours) + " neighbours, where " + blockedNeighbours + " is blocked.");

                foreach (KeyValuePair<Node, Edge> nodeEdgePair in n.Neighbours)
                {
                    if (nodeEdgePair.Key != null)
                    {
                        ConsoleHandler.AddMessage(MessageType.REGULAR, "Edge |" + n.Name + " " + nodeEdgePair.Key.Name + "|: " + nodeEdgePair.Value.ToString() + ".");
                    }
                }

                foreach (KeyValuePair<Node, Edge> nodeEdgePair in n.BlockedNeighbours)
                {
                    if (nodeEdgePair.Key != null)
                    {
                        ConsoleHandler.AddMessage(MessageType.REGULAR, "Blocked edge |" + n.Name + " " + nodeEdgePair.Key.Name + "|: " + nodeEdgePair.Value.ToString() + ".");
                    }
                }
            }
            catch (NodeException e)
            {
                PrintError(e.Message);
            }
        }

        public static void NodeHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"node\" <node>");
        }

        public static void Unblock(String all)
        {
            if (String.Equals(all, "all", StringComparison.CurrentCultureIgnoreCase))
            {
                Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

                int blockedCount = g.BlockedEdges.Count;

                // Unblock all edges
                for (int i = blockedCount - 1; i >= 0; i--)
                {
                    Tuple<Node, Node> edge = g.BlockedEdges[i];
                    g.UnblockEdge(edge.Item1, edge.Item2);

                    ConsoleHandler.AddMessage(MessageType.SUCCESS, "Unblocked edge |" + edge.Item1 + " " + edge.Item2 + "|");
                }

                ConsoleHandler.AddMessage(MessageType.SUCCESS, "Unblocked " + blockedCount + " edges");
            }
            else
            {
                PrintIncorrectUse();
                UnblockHelp();
            }
        }

        public static void Unblock(String nodeOne, String nodeTwo)
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            try
            {
                Node n1 = g.getNode(nodeOne);
                Node n2 = g.getNode(nodeTwo);

                g.UnblockEdge(n1, n2);

                ConsoleHandler.AddMessage(MessageType.SUCCESS, "Unblocked edge |" + n1.Name + " " + n2.Name + "|");
            }
            catch (NodeException e)
            {
                ConsoleHandler.AddMessage(MessageType.ERROR, e.Message);
            }
        }

        public static void UnblockHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"unblock\" \"all\"");
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"unblock\" <node> <node>");
        }

        public static void Clear()
        {
            ConsoleHandler.Clear();
        }

        public static void ClearHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"clear\"");
        }

        public static void Reload()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "Reloading database...");

            Database.Instance.Load();

            PrintSuccess("Database loaded");

            Program.UpdatePositionInformation();
        }

        public static void ReloadHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"reload\"");
        }

        public static void Save()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "Saving database...");

            Database.Instance.Save();

            PrintSuccess("Saved database");
        }

        public static void SaveHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"save\"");
        }

        public static void Exit()
        {
            PrintSuccess("Saving database...");

            Database.Instance.Save();

            PrintSuccess("Database saved, bye bye");

            Thread.Sleep(251);

            Environment.Exit(1);
        }

        public static void ExitHelp()
        {
            ConsoleHandler.AddMessage(MessageType.REGULAR, "\"exit\"");
        }
    }
}
