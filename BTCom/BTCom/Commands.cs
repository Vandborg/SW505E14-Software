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
        private const String COMMAND_DEBUG_NAVIGATE = "debug";
        private const String COMMAND_DELIVER = "deliver";
        private const String COMMAND_FETCH = "fetch";
        private const String COMMAND_NAVIGATE = "navigate";
        private const String COMMAND_POSITION = "position";
        private const String COMMAND_CLEAR = "clear";

        private const String COMMAND_ARGUMENT_CLEAR = "clear";

        private static void printError(string s)
        {
            Console.WriteLine("Error: " + s);
        }

        private static void printSuccess(string s)
        {
            Console.WriteLine("Success: " + s);
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
            // Check if the command is to add a new debug job
            else if (commandIdentifier == COMMAND_DEBUG_NAVIGATE)
            {
                if (arguments == 1)
                {
                    Commands.debug_navigate(commandSplit[0]);
                }
                else
                {
                    debug_navigate_help();
                }
            }
            // Check if the command is "deliver", "fetch" or "navigate"
            else if (commandIdentifier == COMMAND_DELIVER
                     || commandIdentifier == COMMAND_FETCH
                     || commandIdentifier == COMMAND_NAVIGATE)
            {
                if (arguments == 1)
                {
                    movement(commandIdentifier, commandSplit[0]);
                }
                else
                {
                    movement_help();
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
            debug_navigate_help(false);
            movement_help(false);
            position_help(false);
            clear_help(false);
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
                Console.WriteLine("Job #" + job.Key + ": " + job.Value.ToString());
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
                Database.Instance.Save();

                printSuccess("Joblist was cleared");
            }
            else if (subCommand.ToLower() == HELP)
            {
                joblist_help(false);
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
            Console.WriteLine("\"joblist\" [\"remove\" {ID} / \"clear\" / \"help\"]");
        }

        private static void debug_navigate(string directions)
        {
            DebugJob d = new DebugJob(Database.Instance.Data.GetNewDebugJobIdentifier(), BluetoothConnection.TYPE_NAVIGATE_TO, directions);
            Database.Instance.Data.AddDebugJob(d);
            printSuccess("Debug job added");
        }

        private static void debug_navigate_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"debug\" {directions}");
        }

        private static void movement(string type, string node)
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

            byte movementType = 0;

            if (type == COMMAND_DELIVER)
            {
                movementType = BluetoothConnection.TYPE_DELIVER_PALLET;
            }
            else if (type == COMMAND_FETCH)
            {
                movementType = BluetoothConnection.TYPE_FETCH_PALLET;
            }
            else if (type == COMMAND_NAVIGATE)
            {
                movementType = BluetoothConnection.TYPE_NAVIGATE_TO;
            }

            Database.Instance.Data.AddJob((new Job(Database.Instance.Data.GetNewJobIdentifier(), movementType, destination)));
        }

        private static void movement_help(bool incorrect_use = true)
        {
            if (incorrect_use)
            {
                Console.WriteLine("Incorrect use.");
            }
            Console.WriteLine("\"fetch\" / \"deliver\" / \"navigate\" {node}");
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
    }
}
