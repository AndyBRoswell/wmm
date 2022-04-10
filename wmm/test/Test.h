#ifndef WRITING_ASSISTANT_TEST_H
#define WRITING_ASSISTANT_TEST_H

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

            class Widgets {
            public:
                static void MixWithWidgetsTest();
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

#endif // WRITING_ASSISTANT_TEST_H
