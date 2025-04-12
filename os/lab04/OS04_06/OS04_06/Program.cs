class Program
{
    static int Count = 0;
    static void WorkThread()
    {
        for (int i = 0; i < 5000000; ++i)
            Count = Count + 1;
    }
    static void Main(string[] args)
    {
        Thread[] t = new Thread[20];
        for (int i = 0; i < 20; ++i)
        {
            t[i] = new Thread(WorkThread);
            t[i].Start();
        }
        for (int i = 0; i < 20; ++i)
            t[i].Join();
        Console.WriteLine(Count);
        Console.WriteLine(20 * 5000000);
    }
}

//class Program
//{
//    static int Count = 0;
//    static object lockObj = new object(); // объект для синхронизации

//    static void WorkThread()
//    {
//        for (int i = 0; i < 5000000; ++i)
//        {
//            lock (lockObj) // блокировка
//            {
//                Count = Count + 1;
//            }
//        }
//    }

//    static void Main(string[] args)
//    {
//        Thread[] t = new Thread[20];
//        for (int i = 0; i < 20; ++i)
//        {
//            t[i] = new Thread(WorkThread);
//            t[i].Start();
//        }
//        for (int i = 0; i < 20; ++i)
//            t[i].Join();

//        Console.WriteLine(Count); // вывод результата
//        Console.WriteLine(20 * 5000000); // вывод ожидаемого результата
//    }
//}
