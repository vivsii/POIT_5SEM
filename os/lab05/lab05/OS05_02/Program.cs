using System;
using System.Diagnostics;
using System.Threading;

class Program
{
    // Количество потоков
    const int ThreadCount = 10;
    static void Main()
    {
        Console.WriteLine("Евсеенко Виктория");

        Process.GetCurrentProcess().ProcessorAffinity = (IntPtr)15;

        Thread[] threads = new Thread[ThreadCount];

        for (int i = 0; i < ThreadCount; ++i)
        {
            object threadIndex = i;

            threads[i] = new Thread(WorkThread);

            switch (i % 3)
            {
                case 0:
                    threads[i].Priority = ThreadPriority.Lowest;
                    break;
                case 2:
                    threads[i].Priority = ThreadPriority.Highest;
                    break;
                default:
                    break;
            }

            threads[i].Start(threadIndex);
        }

        foreach (var thread in threads)
        {
            thread.Join();
        }
    }
    static void WorkThread(object o)
    {
        int threadIndex = (int)o; 

        for (int i = 0; i < 75; i++)
        {
            Console.WriteLine($"Поток {threadIndex} - Итерация {i + 1}");

            MySleep();
        }
    }
    static void MySleep()
    {
        Thread.Sleep(1);
    }
}
