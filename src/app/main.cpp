#include "main.h"

int main(int argc, char * argv[])
{
    Q_INIT_RESOURCE(resources);

    /*
     * On Windows an application is either a GUI application or Console application.
     * This application is a rare GUI and CLI application hybrid.
     *
     * The application is compiled with "CONFIG += CONSOLE" to support the native console mode.
     * When the app is executed without command line args, it will start a console in GUI mode.
     *
     * If CLI args are found, the application reacts as a console application.
     * else it runs as a normal QtGUI application.
     */

    if (argc > 1) { // first arg is the executable itself
        QApplication app(argc, argv);
        QApplication::setApplicationName(APP_NAME);
        QApplication::setApplicationVersion(APP_VERSION);

        ServerControlPanel::CLI *cli = new ServerControlPanel::CLI;
        cli->handleCommandLineArguments();

        return app.exec();
    }

    // Initialize Qt application
    QApplication app(argc, argv);

    /*
     * By calling FreeConsole(), we close this console immediately after starting the GUI.
     * That results in a short console flickering.
     *
     * Yes, it's annoying - but feel free to contribute a better solution.
     * Rules: one executable; not embedded exe; not the devenv.com solution.
     * Ok, now its your turn... :)
     *
     * For more information, see:
     * https://github.com/WPN-XM/WPN-XM/issues/39
     */
    FreeConsole();

    // Single Instance Check
    ServerControlPanel::Main::exitIfAlreadyRunning();

    // Application Meta Data and Settings
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName("Jens-André Koch");
    app.setOrganizationDomain("http://wpn-xm.org/");
    app.setWindowIcon(QIcon(":/wpnxm.ico"));

    // do not leave application, until Quit is clicked in the tray menu
    app.setQuitOnLastWindowClosed(false);

    /**
     * Assume the screen has a resolution of 96 DPI rather than using
     * the OS-provided resolution. This will cause font rendering to
     * be consistent in pixels-per-point across devices rather than
     * defining 1 point as 1/72 inch.
     */
    app.setAttribute(Qt::AA_Use96Dpi, true);

    /**
     * @brief Set Style: "Fusion Dark"
     */
    //app.setStyle(QStyleFactory::create("Fusion"));
    /*
    QPalette p;
    p = app.palette();
    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::Button, QColor(53,53,53));
    p.setColor(QPalette::Highlight, QColor(142,45,197));
    p.setColor(QPalette::ButtonText, QColor(255,255,255));
    app.setPalette(p);*/

    // Activate SplashScreen
    #ifndef QT_DEBUG
    ServerControlPanel::SplashScreen splash(QPixmap(), Qt::WindowStaysOnTopHint);
    splash.setProgress(0);
    splash.show();
    #endif

    #ifndef QT_DEBUG
    splash.setProgress(50);
    #endif

    // Initialize Application main window and show it
    ServerControlPanel::MainWindow mainWindow;
    mainWindow.show();

    #ifndef QT_DEBUG
    splash.setProgress(100);
    #endif

     // Deactivate SplashScreen
    #ifndef QT_DEBUG
    splash.finish(&mainWindow);
    #endif

    // enter the Qt Event loop here
    return app.exec();
}

namespace ServerControlPanel
{
    /*
     * Single Instance Check
     * Although some people tend to solve this problem by using QtSingleApplication,
     * this approach uses a GUID stored into shared memory.
     */
    void Main::exitIfAlreadyRunning()
    {
        // Set GUID for WPN-XM Server Control Panel to memory
        // It needs to be "static", because the QSharedMemory instance gets destroyed
        // at the end of the function and so does the shared memory segment.
        static QSharedMemory shared("004d54f6-7d00-4478-b612-f242f081b023");

        // already running
        if( !shared.create( 512, QSharedMemory::ReadWrite) )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(APP_NAME);
          msgBox.setText( QObject::tr("WPN-XM is already running.") );
          msgBox.setIcon( QMessageBox::Critical );
          msgBox.exec();

          exit(0);
        }
    }

}
