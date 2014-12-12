using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace BTCom
{
    public static class ConsoleHandler
    {
        private static int old_top = 2;
        private static bool clear_command = false;

        public static String DNS = "PALL-E";

        private static readonly List<KeyValuePair<MessageType, String>> messageList = new List<KeyValuePair<MessageType, string>>();

        public static void AddMessage(MessageType type, String message)
        {
            messageList.Add(new KeyValuePair<MessageType, string>(type, message));
        }

        public static void PrintMessages()
        {
            Clear();

            while (true)
            {
                if (messageList.Count > 0)
                {
                    KeyValuePair<MessageType, String> message = messageList.First();

                    // Set the console color depending on message type
                    if (message.Key == MessageType.REGULAR)
                    {
                        Console.ForegroundColor = ConsoleColor.White;
                    }
                    else if (message.Key == MessageType.SUCCESS)
                    {
                        Console.ForegroundColor = ConsoleColor.Green;
                    }
                    else if (message.Key == MessageType.ERROR)
                    {
                        Console.ForegroundColor = ConsoleColor.Red;
                    }
                    else if (message.Key == MessageType.BLUETOOTH)
                    {
                        Console.ForegroundColor = ConsoleColor.DarkCyan;
                    }
                    else
                    {
                        Console.ForegroundColor = ConsoleColor.White;
                    }

                    // Display the message
                    Console.SetCursorPosition(0, old_top + 2);
                    Console.Write(new string(' ', Console.WindowWidth));
                    Console.SetCursorPosition(0, old_top + 2);
                    Console.WriteLine(message.Value);
                    old_top++;
                    Console.ResetColor();

                    if (Console.CursorTop > 1)
                    {
                        // Clear the next line 
                        Console.SetCursorPosition(0, old_top + 2);
                        Console.WriteLine(new string(' ', Console.WindowWidth));
                        Console.WriteLine(new string(' ', Console.WindowWidth));
                        Console.WriteLine(new string(' ', Console.WindowWidth)); 
                    }
                    
                    old_top = old_top % (Console.WindowHeight - 3);
                    
                    messageList.Remove(message);

                    Console.SetCursorPosition(0, 0);
                }
                
                if (clear_command)
                {
                    // Clear the first line of the console
                    Console.SetCursorPosition(0, 0);
                    Console.Write(new string(' ', Console.WindowWidth));
                    Console.SetCursorPosition(0, 0);

                    clear_command = false;
                }

                // Other threads should be able to run after each iteration
                Thread.Yield();
            }
        }

        public static void Clear()
        {
            Console.Clear();

            Console.SetCursorPosition(0, 1);
            Console.Write(new string('-', Console.WindowWidth));
            Console.SetCursorPosition(0, 2);

            Console.SetCursorPosition(0, 2);

            old_top = 2;
        }

        public static void ClearCommand()
        {
            clear_command = true;
        }
    }
}