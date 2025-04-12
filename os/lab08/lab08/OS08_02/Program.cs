using System;
using System.Diagnostics;
using System.Threading;

namespace OS08_02
{
    class Program
    {
        private static byte[] memoryObject;
        private static Random random = new Random();

        static void Main(string[] args)
        {
            int objectSizeMB = 128;

            while (true)
            {
                memoryObject = new byte[objectSizeMB * 1024 * 1024];
                Thread fillThread = new Thread(FillMemory);
                fillThread.Start();
                long usedMemory = Process.GetCurrentProcess().PrivateMemorySize64;
                Console.WriteLine($"Используемая память: {usedMemory / (1024 * 1024)} МБ");
                Thread.Sleep(5000);
            }
        }

        static void FillMemory()
        {
            for (int i = 0; i < memoryObject.Length; i++)
            {
                memoryObject[i] = (byte)random.Next(256);
            }
        }
    }
}