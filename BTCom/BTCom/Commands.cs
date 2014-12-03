using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;

namespace BTCom
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
        private const String COMMAND_DIRECTIONS = "directions";
        private const String COMMAND_DEBUG = "debug";
        private const String COMMAND_FETCH = "fetch";
        private const String COMMAND_NAVIGATE = "navigate";
        private const String COMMAND_POSITION = "position";
        private const String COMMAND_CURRENTJOB = "currentjob";
        private const String COMMAND_PALLETLIST = "palletlist";
        private const String COMMAND_PAYLOAD = "payload";
        private const String COMMAND_CLEAR = "clear";
        private const String COMMAND_SAVE = "save";
        private const String COMMAND_EXIT = "exit";

        private const String COMMAND_ARGUMENT_CLEAR = "clear";

        private static void printError(string s)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine("Error: " + s);
            Console.ResetColor();
        }

        private static void printSuccess(string s)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Success: " + s);
            Console.ResetColor();
        }

        private static void printIncorrectUse()
        {
            Console.ForegroundColor = ConsoleColor.DarkRed;
            Console.WriteLine("Incorrect use.");
            Console.ResetColor();
        }

        private static void printInvalidCommand()
        {
            printError("Invalid command. Type \"help\" for help.");
        }

        public static void execute()
        {
            while (true)
            {
                Commands.execute(Console.ReadLine());
            }
        }

        private static void execute(string command)
        {
            command = command.ToLower();

            // No command provided, user must need help
            if (String.IsNullOrEmpty(command))
            {
                printInvalidCommand();
                return;
            }

            // Split the string into command and arguments
            List<String> commandAndArguments = command.Split('-').ToList();

            // No command provided, user must need help
            if (String.IsNullOrEmpty(commandAndArguments[0]))
            {
                printInvalidCommand();
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
                clear();
            }

            // Check if there are unprocessed arguments
            if (argumentList.Count > 0)
            {
                printError("Unknown command arguments: ");

                foreach (string s in argumentList)
                {
                    if (s != argumentList.FirstOrDefault())
                    {
                        Console.Write(", ");
                    }

                    Console.Write("'" + s + "'"); 
                }

                Console.Write("\n");

                return;
            }

            // Check if the command is "help"
            if (commandIdentifier == HELP)
            {
                if (arguments == 0)
                {
                    Commands.help();    
                }
                else
                {
                    printIncorrectUse();
                    help_help();
                }
            } 
            else if (commandIdentifier == COMMAND_STATUS)
            {
                if (arguments == 0)
                {
                    status();
                }
                else
                {
                    printIncorrectUse();
                    status_help();
                }
            }
            // Check if the command is "joblist"
            else if (commandIdentifier == COMMAND_JOBLIST)
            {
                if (arguments == 0)
                {
                    Commands.joblist();
                }
                else if (arguments == 1)
                {
                    Commands.joblist(commandSplit[0]);
                }
                else if (arguments == 2)
                {
                    Commands.joblist(commandSplit[0], commandSplit[1]);
                }
                else
                {
                    printIncorrectUse();
                    joblist_help();
                }
            }
            else if (commandIdentifier == COMMAND_CURRENTJOB)
            {
                if (arguments == 0)
                {
                    Commands.current_job();
                }
                else
                {
                    printIncorrectUse();
                    current_job_help();
                }
            }
            // Check if the command is "deliver", "fetch" or "navigate"
            else if (commandIdentifier == COMMAND_DELIVER
                     || commandIdentifier == COMMAND_FETCH
                     || commandIdentifier == COMMAND_NAVIGATE)
            {
                if (arguments == 2)
                {
                    if (commandSplit[0] == "node")
                    {
                        moveNode(commandIdentifier, commandSplit[1]);    
                    }
                    else if(commandSplit[0] == "pallet")
                    {
                        movePallet(commandIdentifier, commandSplit[1]);
                    }
                    else
                    {
                        printIncorrectUse();
                        moveNode_help();
                        movePallet_help();
                    }
                }
                else
                {
                    printIncorrectUse();
                    moveNode_help();
                    movePallet_help();
                }
            }
            else if (commandIdentifier == COMMAND_DIRECTIONS)
            {
                if (arguments == 1)
                {
                    directions(commandSplit[0]);
                }
            }
            else if (commandIdentifier == COMMAND_DEBUG)
            {
                if (arguments == 1)
                {
                    debug(commandSplit[0]);
                }
            }
            else if (commandIdentifier == COMMAND_POSITION)
            {
                if (arguments == 0)
                {
                    position();
                }
                else if (arguments == 2)
                {
                    position(commandSplit[0], commandSplit[1]);
                }
                else
                {
                    printIncorrectUse();
                    position_help();
                }
            }
            else if (commandIdentifier == COMMAND_CLEAR)
            {
                if (arguments == 0)
                {
                    Commands.clear();
                }
                else
                {
                    printIncorrectUse();
                    clear_help();
                }
            }
            else if (commandIdentifier == COMMAND_PALLETLIST)
            {
                if (arguments == 0)
                {
                    Commands.palletlist();
                }
                else if (arguments == 1)
                {
                    Commands.palletlist(commandSplit[0]);
                }
                else if (arguments == 3)
                {
                    Commands.palletlist(commandSplit[0], commandSplit[1], commandSplit[2]);
                }
                else
                {
                    printIncorrectUse();
                    palletlist_help();
                }
            }
            else if (commandIdentifier == COMMAND_PAYLOAD)
            {
                if (arguments == 0)
                {
                    payload();
                }
                else if (arguments == 1)
                {
                    payload(commandSplit[0]);
                }
                else
                {
                    printIncorrectUse();
                    payload_help();
                }
            }
            else if (commandIdentifier == COMMAND_SAVE)
            {
                if (arguments == 0)
                {
                    Commands.save();
                }
                else
                {
                    printIncorrectUse();
                    save_help();
                }
            }
            else if (commandIdentifier == COMMAND_EXIT)
            {
                if (arguments == 0)
                {
                    Commands.exit();
                }
                else
                {
                    printIncorrectUse();
                    exit_help();
                }
            }
            // Some unknown command, show the help
            else
            {
                printInvalidCommand();
            }
        }

        private static void help()
        {
            help_help();
            status_help();
            joblist_help();
            moveNode_help();
            movePallet_help();
            directions_help();
            debug_help();
            position_help();
            palletlist_help();
            payload_help();
            clear_help();
            save_help();
            exit_help();
        }

        private static void help_help()
        {
            Console.WriteLine("\"help\"");
        }

        public static void status()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            Console.WriteLine("Status: " + f.Status);
        }

        public static void status_help()
        {
            Console.WriteLine("\"status\"");
        }

        public static void joblist()
        {
            Console.WriteLine("Current Joblist:");

            int c = 0;
            foreach (KeyValuePair<int, Job> job in Database.Instance.Data.Jobs)
            {
                Console.WriteLine(job.Value.ToString());
                c++;
            }

            if (c == 0)
            {
                Console.WriteLine("(Empty)");
            }
            
        }

        public static void joblist(string subCommand)
        {
            if (subCommand.ToLower() == SUBCOMMAND_CLEAR)
            {
                // Remove all jobs
                Database.Instance.Data.Jobs = new Dictionary<int, Job>();

                printSuccess("Joblist was cleared");
            }
            else
            {
                printIncorrectUse();
                joblist_help();
            }
        }

        public static void joblist(string subCommand, string identifier)
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
                            printSuccess("Job removed");
                        }
                        else
                        {
                            printError("Could not find job");
                        }
                    }
                    else
                    {
                        printError("Could not find job with identifier '" + i + "'");
                    }
                }
                else
                {
                    printError("{ID} is not a correct identifier");
                    joblist_help();
                }
            }
            else
            {
                printIncorrectUse();
                joblist_help();
            }
        }

        private static void joblist_help()
        {
            Console.WriteLine("\"joblist\" [\"remove\" {ID} / \"clear\"]");
        }

        private static void moveNode(string type, string node)
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            Node destination = null;

            try
            {
                destination = g.getNode(node);
            }
            catch (NodeException e)
            {
                printError(e.Message);
                return;
            }

            if (type == COMMAND_DELIVER)
            {
                try
                {
                    Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), destination, PalletJobType.deliver));
                    printSuccess("Job added");
                }
                catch (Exception e)
                {
                    printError(e.Message);
                }
            }
            else if (type == COMMAND_FETCH)
            {
                try 
                {
                    Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), destination, PalletJobType.fetch));
                    printSuccess("Job added");
                }
                catch (Exception e)
                {
                    printError(e.Message);
                }
            }
            else if (type == COMMAND_NAVIGATE)
            {
                try
                {
                    Database.Instance.Data.AddJob(new NavigateJob(Database.Instance.Data.GetNewJobIdentifier(), destination));
                    printSuccess("Job added");
                }
                catch (Exception e)
                {
                    printError(e.Message);
                }
            }
            else
            {
                printIncorrectUse();
                moveNode_help();
            }
        }

        private static void moveNode_help()
        {
            Console.WriteLine("[\"fetch\" / \"deliver\" / \"navigate\"] \"node\" {node}");
        }

        private static void movePallet(string type, string palletName)
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
                printError("Could not find pallet with name '" + palletName + "'");
                return;
            }

            if (type == COMMAND_DELIVER)
            {
                try
                {
                    Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), pallet, PalletJobType.deliver));
                    printSuccess("Job added");
                }
                catch (Exception e)
                {
                    printError(e.Message);
                }
            }
            else if (type == COMMAND_FETCH)
            {
                try 
                {
                    Database.Instance.Data.AddJob(new PalletJob(Database.Instance.Data.GetNewJobIdentifier(), pallet, PalletJobType.fetch));
                    printSuccess("Job added");
                }
                catch (Exception e)
                {
                    printError(e.Message);
                }
            }
            else if (type == COMMAND_NAVIGATE)
            {
                printIncorrectUse();
                movePallet_help();
            }
        }

        private static void movePallet_help()
        {
            Console.WriteLine("\"fetch\" \"pallet\" {pallet}");
        }
       
        private static void directions(string directions)
        {
            try
            {
                Database.Instance.Data.AddJob(new NavigateJob(Database.Instance.Data.GetNewJobIdentifier(), directions));
                printSuccess("Job added");
            }
            catch (Exception e)
            {
                printError(e.Message);
            }
            
        }

        private static void directions_help()
        {
            Console.WriteLine("\"directions\" {L | S | R}*");
        }

        private static void debug(string directions)
        {
            try
            {
                DebugJob dj = new DebugJob(Database.Instance.Data.GetNewJobIdentifier(), directions);
                Database.Instance.Data.AddJob(dj);
            }
            catch (FormatException e)
            {
                printError(e.Message);
            }
            
        }

        private static void debug_help()
        {
            Console.WriteLine("\"debug\" {L | S | R}*");
        }

        private static void position()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            Console.WriteLine("Front-node: " + f.FrontNode.Name);
            Console.WriteLine("Rear-node:  " + f.RearNode.Name);
        }

        private static void position(string frontNode, string rearNode)
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
                printError(e.Message);
                return;
            }

            try
            {
                f.UpdateNodes(front, rear);
            }
            catch (NodeException e)
            {
                printError(e.Message);
                return;
            }

            printSuccess("Position updated");
        }

        private static void position_help()
        {
            Console.WriteLine("\"position\" [FrontNode RearNode]");
        }

        private static void current_job()
        {
            if (BluetoothConnection.CurrentJob != null)
            {
                Console.WriteLine("Current job: '" + BluetoothConnection.CurrentJob.ToString() + "'");
            }
            else
            {
                Console.WriteLine("No current job");
            }
        }

        private static void current_job_help()
        {
            Console.WriteLine("\"currentjob\"");
        }

        private static void palletlist()
        {
            Dictionary<int, Pallet> pallets = Database.Instance.Data.Pallets;

            if (pallets.Count == 0)
            {
                Console.WriteLine("No pallets");
                return;
            }

            foreach (KeyValuePair<int, Pallet> pallet in pallets)
            {
                Console.WriteLine("#" + pallet.Key + " - " + pallet.Value.ToString());
            }
        }

        private static void palletlist(string subcommand)
        {
            if (subcommand == COMMAND_CLEAR)
            {
                Database.Instance.Data.Pallets = new Dictionary<int, Pallet>();

                printSuccess("Pallet list cleared");
            }
            else
            {
                printIncorrectUse();
                palletlist_help();
            }
        }

        private static void palletlist(string subcommand, string name, string location)
        {
            if (subcommand == SUBCOMMAND_ADD)
            {
                if (location == "forklift")
                {
                    Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

                    if (f.HasPallet)
                    {
                        printError("Forklift is already carrying pallet: '" + f.Payload.Name + "'");
                        return;
                    }

                    Pallet p = new Pallet(Database.Instance.Data.GetNewPalletIdentifier(), name.ToUpper(), null);
                    Database.Instance.Data.AddPallet(p);
                    f.Payload = p;

                    printSuccess("Pallet added.");
                }
                else
                {
                    Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

                    try
                    {
                        Node n = g.getNode(location);
                        Pallet p = new Pallet(Database.Instance.Data.GetNewPalletIdentifier(), name.ToUpper(), n);
                        Database.Instance.Data.AddPallet(p);

                        printSuccess("Pallet added.");
                    }
                    catch (NodeException e)
                    {
                        printError(e.Message);
                    }
                }
            }
            else
            {
                printIncorrectUse();
                palletlist_help();
            }
        }

        private static void payload()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            if (f.HasPallet)
            {
                Console.WriteLine(f.Payload.ToString());
            }
            else
            {
                Console.WriteLine("No current payload");
            }
        }

        private static void payload(String newPayload)
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            if (f.HasPallet)
            {
                printError("Forklift is already carrying pallet: '" + f.Payload.Name + "'");
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
                printError("Could not find pallet with name '" + newPayload + "'");
                return;
            }

            Database.Instance.Data.Forklifts.FirstOrDefault().Value.Payload = pallet;
            printSuccess("Payload updated");
        }

        private static void payload_help()
        {
            Console.WriteLine("\"payload\" [\"pallet\"]");
        }

        private static void palletlist_help()
        {
            Console.WriteLine("\"palletlist\" [\"clear\" / (\"add\" {name} (\"forklift\" / {node})) / (\"remove\" {pallet})]");
        }

        private static void clear()
        {
            Console.Clear();
        }

        private static void clear_help()
        {
            Console.WriteLine("\"clear\"");
        }

        private static void save()
        {
            Database.Instance.Save();

            printSuccess("Saved database");
        }

        private static void save_help()
        {
            Console.WriteLine("\"save\"");
        }

        private static void exit()
        {
            printSuccess("Saving database...");

            Database.Instance.Save();

            printSuccess("Database saved, bye bye");

            System.Threading.Thread.Sleep(251);

            Environment.Exit(1);
        }

        private static void exit_help()
        {
            Console.WriteLine("\"exit\"");
        }
    }
}
