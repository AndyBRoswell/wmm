#ifndef WRITING_MATERIALS_MANAGER_TEST_H
#define WRITING_MATERIALS_MANAGER_TEST_H

#include <string>

namespace WritingMaterialsManager {
    class Test {
    public:
        constexpr static char Sep[] = "===============";

        static int Start();

        static int SimpleTest();

        // Qt tests
        class Qt {
        public:
            static void EncodingOfFileRW();
            static void GetSystemLocale();
            static void InterProcessCommunication();

            class Widgets {
            public:
                static void Demo();
            };

            class Quick {
            public:
                static void TextAreaKeyEvent();
            };
        };

        // Other 3rd-party lib tests
        class simdjson {
        public:
            static void Demo();
        };

        class RapidJSON {
        public:
            static void Demo();
        };

        class mongocxx {
        public:
            static void Tutorial();

            static void CustomDataDemo();
        };

        class DuckX {
        public:
            static void QuickStart();
        };
    private:
        static std::string LastStartedFn;
        static std::string LastFinishedFn;
    };
}

#endif // WRITING_MATERIALS_MANAGER_TEST_H
