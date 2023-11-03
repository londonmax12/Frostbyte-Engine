namespace Frostbyte {
    class Application {
    public:
        bool IsRunning() { return m_Running; }

        // Pure virtual functions to be overwritten
        virtual bool OnInit() = 0;
        virtual void OnUpdate() = 0;

    private:
        bool m_Running = true;
    };
}