using System;
using System.Diagnostics;

class Program
{
    static void Main(string[] args)
    {
        // Пример использования MySleep
        int ms = 10000; // 10 секунд
        Stopwatch sw = new Stopwatch();
        sw.Start();

        double result = MySleep(ms);

        sw.Stop();
        Console.WriteLine("Время работы: {0} миллисекунд", sw.ElapsedMilliseconds);
        Console.WriteLine("Результат вычислений: " + result);
    }

    static Double MySleep(int ms)
    {
        Double Sum = 0, Temp;
        for (int t = 0; t < ms; ++t)
        {
            Temp = 0.711 + (Double)t / 10000.0;
            Double a, b, c, d, e, nt;
            for (int k = 0; k < 65000; ++k) // Увеличено до 65000
            {
                nt = Temp - k / 27000.0;
                a = Math.Sin(nt);
                b = Math.Cos(nt);
                c = Math.Cos(nt / 2.0);
                d = Math.Sin(nt / 2);
                e = Math.Abs(1.0 - a * a - b * b) + Math.Abs(1.0 - c * c - d * d);
                Sum += e;
            }
        }
        return Sum;
    }
}