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

        private const String JOBLIST_REMOVE = "remove";
        private const String JOBLIST_CLEAR = "clear";

        private const String COMMAND_STATUS = "status";
        private const String COMMAND_JOBLIST = "joblist";
        private const String COMMAND_DELIVER = "deliver";
        private const String COMMAND_DIRECTIONS = "directions";
        private const String COMMAND_DEBUG = "debug";
        private const String COMMAND_FETCH = "fetch";
        private const String COMMAND_NAVIGATE = "navigate";
        private const String COMMAND_POSITION = "position";
        private const String COMMAND_CURRENTJOB = "currentjob";
        private const String COMMAND_LIST = "list";
        private const String COMMAND_LIST_PALLETS = "pallets";
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
                Console.Write("Unknown command arguments: ");

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
                        moveNode_help();
                        movePallet_help(false);
                    }
                }
                else
                {
                    moveNode_help();
                    movePallet_help(false);
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
                    clear_help();
                }
            }
            else if (commandIdentifier == COMMAND_LIST)
            {
                if (arguments == 1)
                {
                    Commands.list(commandSplit[0]);
                }
                else
                {
                    clear_help();
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
            help_help(false);
            status_help(false);
            joblist_help(false);
            moveNode_help(false);
            movePallet_help(false);
            directions_help(false);
            debug_help(false);
            position_help(false);
            list_help(false);
            clear_help(false);
            save_help(false);
            exit_help(false);
        }

        private static void help_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"help\"");
        }

        public static void status()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            Console.WriteLine("Status: " + f.Status);
        }

        public static void status_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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
            if (subCommand.ToLower() == JOBLIST_CLEAR)
            {
                // Remove all jobs
                Database.Instance.Data.Jobs = new Dictionary<int, Job>();

                printSuccess("Joblist was cleared");
            }
            else
            {
                joblist_help();
            }
        }

        public static void joblist(string subCommand, string identifier)
        {
            if (subCommand.ToLower() == JOBLIST_REMOVE)
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
                joblist_help();
            }
        }

        private static void joblist_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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
        }

        private static void moveNode_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("[\"fetch\" / \"deliver\" / \"navigate\"] \"node\" {node}");
        }

        private static void movePallet(string type, string palletName)
        {
            Pallet pallet = null;

            foreach (KeyValuePair<int, Pallet> palletPair in Database.Instance.Data.Pallets)
            {
                if (palletPair.Value.Name.ToLower() == palletName.ToLower())
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
                movePallet_help();
            }
        }

        private static void movePallet_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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

        private static void directions_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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

        private static void debug_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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

        private static void position_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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

        private static void current_job_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"currentjob\"");
        }

        private static void list(string listIdentifier)
        {
            if (listIdentifier == COMMAND_LIST_PALLETS)
            {
                Dictionary<int, Pallet> pallets = Database.Instance.Data.Pallets;

                if (pallets.Count == 0)
                {
                    Console.WriteLine("No pallets");
                }

                foreach (KeyValuePair<int, Pallet> pallet in pallets)
                {
                    Console.WriteLine("#" + pallet.Key + " - " + pallet.Value.ToString());
                }
            }
            else
            {
                list_help();
            }
        }

        private static void list_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"list\" \"pallets\"");
        }

        private static void clear()
        {
            Console.Clear();
        }

        private static void clear_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"clear\"");
        }

        private static void save()
        {
            Database.Instance.Save();

            printSuccess("Saved database");
        }

        private static void save_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
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

        private static void exit_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"exit\"");
        }
    }
}
