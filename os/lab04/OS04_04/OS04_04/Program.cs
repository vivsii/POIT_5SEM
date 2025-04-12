using System;
using System.Threading;
using System.Diagnostics; // Для Stopwatch

class Program
{
    static void ThreadZed()
    {
        Stopwatch stopwatch = Stopwatch.StartNew();
        for (int i = 0; i < 10; i++)
        {
            Console.Write(" (Z-{0}) {1} сек ", Thread.CurrentThread.ManagedThreadId, stopwatch.Elapsed.Seconds);
            Thread.Sleep(1000);
        }
        Console.WriteLine(" Поток Z завершается через {0} секунд", stopwatch.Elapsed.Seconds);
    }

    static void ThreadWithParam(object o)
    {
        Stopwatch stopwatch = Stopwatch.StartNew();
        for (int i = 0; i < 20; i++)
        {
            Console.Write(" ({0}-{1}) {2} сек ", o.ToString(), Thread.CurrentThread.ManagedThreadId, stopwatch.Elapsed.Seconds);
            Thread.Sleep(1000);
        }
        Console.WriteLine(" Поток {0} завершается через {1} секунд", o.ToString(), stopwatch.Elapsed.Seconds);
    }

    static void Main(string[] args)
    {
        Stopwatch mainStopwatch = new Stopwatch();
        mainStopwatch.Start();

        var t1 = new Thread(ThreadZed);
        var t1a = new Thread(ThreadWithParam);
        var t1b = new Thread(ThreadWithParam);

        t1.IsBackground = false;  // false для п.11
        t1a.IsBackground = false;  // true для п.12
        t1b.IsBackground = true;  // true для третьего потока

        t1.Start();
        t1a.Start("Виктория");  // Имя
        t1b.Start("Евсеенко"); // Фамилия

        // Главный поток работает 5 секунд
        for (int i = 0; i < 5; i++)
        {
            Console.Write(" (*-{0}) {1} сек ", Thread.CurrentThread.ManagedThreadId, mainStopwatch.Elapsed.Seconds);
            Thread.Sleep(1000);
        }

        Console.WriteLine("Главный поток завершается через {0} секунд", mainStopwatch.Elapsed.Seconds);

        mainStopwatch.Stop();
    }
}
