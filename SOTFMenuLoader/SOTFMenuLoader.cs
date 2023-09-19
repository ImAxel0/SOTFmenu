using RedLoader.Utils;
using SonsSdk;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.VisualBasic.FileIO;
using RedLoader;
using UnityEngine;
using Il2CppSystem.Threading;

namespace SOTFMenuLoader;

public class SOTFMenuLoader : SonsMod
{
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr LoadLibrary(string lib);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern void FreeLibrary(IntPtr module);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr GetProcAddress(IntPtr module, string proc);

    static bool LoadState;

    protected override void OnInitializeMod()
    {
        var documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
        if (!Directory.Exists(Path.Combine(documentsPath, "SOTF")))
        {
            RLog.Msg("Installing SOTFmenu...");
            FileSystem.CopyDirectory(Path.Combine(LoaderEnvironment.ModsDirectory, @"SOTFMenuLoader\Copy"), documentsPath);
        }
        else
            RLog.Msg("SOTFmenu already installed");
    }

    protected override void OnGameStart()
    {
        if (LoadState) return;

        IntPtr module = LoadLibrary(Path.Combine(LoaderEnvironment.ModsDirectory, @"SOTFMenuLoader\SOTFmenu.dll"));
        if (module == IntPtr.Zero) // error handling
        {
            Console.WriteLine($"Could not load library: {Marshal.GetLastWin32Error()}");
            return;
        }
        RLog.Msg("SOTFmenu loaded");
        LoadState = true;
    }
}