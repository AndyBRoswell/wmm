// std
#include <unordered_map>

// Qt
#include <QSignalSpy>
#include <QTest>
#include <QWaitCondition>

// files to be tested
#include "src/MongoDBConsole.h"
#include "src/PythonInteractor.h"

class test : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 3");
    }

    void MongoShAccessor__basic() {
        namespace wmm = WritingMaterialsManager;
        using namespace std::chrono_literals;

        wmm::MongoShAccessor mongosh_accessor;
        { // show dbs
            QSignalSpy MoreResult_signal_spy(&mongosh_accessor, &wmm::MongoShAccessor::MoreResult);
            QSignalSpy NoMoreResult_signal_spy(&mongosh_accessor, &wmm::MongoShAccessor::NoMoreResult);
            QString result;
            QObject::connect(&mongosh_accessor, &wmm::MongoShAccessor::MoreResult, [&](const QString& result_fragment) {
                result += result_fragment;
                });
            QObject::connect(&mongosh_accessor, &wmm::MongoShAccessor::NoMoreResult, [&]() {
                QCOMPARE_GT(MoreResult_signal_spy.count(), 0);
                QCOMPARE(NoMoreResult_signal_spy.count(), 1);
                QCOMPARE_GT(result.size(), 0);
                QCOMPARE_GT(result.indexOf("admin"), -1);
                QCOMPARE_GT(result.indexOf("config"), -1);
                QCOMPARE_GT(result.indexOf("local"), -1);
                });
            mongosh_accessor.Execute("show dbs\n");
        }
        { // JSON operations

        }
    }

    void MongoDBConsole__basic() {
        namespace wmm = WritingMaterialsManager;

        //wmm::MongoDBConsole MongoDB_console;
        //wmm::TreeEditor tree_editor;
        //MongoDB_console.AddAssociatedEditor(&tree_editor);
        { // show dbs
            //MongoDB_console.ExecuteShellCommand();
            //QSignalSpy spy(&MongoDB_console, &wmm::MongoDBConsole::NewShellCommand);
            //QVERIFY(spy.isValid());
        }
        { // JSON operations

        }
    }

    // NOTE: run py-deploy script in the root dir with its sample (call it with "-help" argument) of WMM before running this test.
    void PythonAccessor__basic() {
        namespace wmm = WritingMaterialsManager;

        const std::unordered_map<QByteArray, std::shared_ptr<wmm::PythonAccessor>> Python_accessor = {
            { "latest", std::make_shared<wmm::PythonAccessor>("py/venv/latest/Scripts/python") },
            { "3.8", std::make_shared<wmm::PythonAccessor>("py/venv/3.8/Scripts/python") },
        };
        QString result;

        for (const auto& p : Python_accessor) {
            QObject::connect(p.second.get(), &wmm::PythonAccessor::MoreResult, [&](const QString& result_fragment) {
                result += result_fragment;
                });
        }

        { // hello world
            for (const auto& e : Python_accessor) {
                const auto p = e.second.get();
                QSignalSpy MoreResult_signal_spy(p, &wmm::PythonAccessor::MoreResult);
                QSignalSpy NoMoreResult_signal_spy(p, &wmm::PythonAccessor::NoMoreResult);
                const auto conn = QObject::connect(p, &wmm::PythonAccessor::NoMoreResult, [&]() {
                    QCOMPARE_GT(MoreResult_signal_spy.count(), 0);
                    QCOMPARE(NoMoreResult_signal_spy.count(), 1);
                    QCOMPARE(result.trimmed(), "Hello, World!");
                    });
                p->Execute(R"(print("Hello, World!"))");
                QObject::disconnect(conn);
                result.clear();
            }
        }
        { // NLP libraries
            for (const auto& e : Python_accessor) {
                const auto p = e.second.get();
                QSignalSpy MoreResult_signal_spy(p, &wmm::PythonAccessor::MoreResult);
                QSignalSpy NoMoreResult_signal_spy(p, &wmm::PythonAccessor::NoMoreResult);
                const auto conn = QObject::connect(p, &wmm::PythonAccessor::NoMoreResult, [&]() {
                    const QRegularExpression re(R"(\['动听.+\[1\.0,)");
                    QVERIFY(re.match(result).hasMatch());
                    });
                p->Execute(
                    "import synonyms\n"
                    "print(synonyms.nearby('动听',10))\n"
                );
                QObject::disconnect(conn);
                result.clear();
            }
            {
                const QString text =
                    "　　走到桥上，桥下，流水汤汤，一张落叶正飘下来，擦着水皮掠过一阵，又象被吸住了一样贴在水面上，顺水流去。"
                    "这条河本来被污染得很厉害，淤泥积得几乎要堵塞河道。这些天来，水量倒增加了。"
                    "我把烟头扔进河里，又摸出一支烟，刚凑到嘴边，忽然肩头被撞了一下，那支烟也掉在地上。"
                    "我扭头一看，是个醉醺醺的流浪汉，手上拎了一瓶酒。他见我看了他一眼，瞪大了眼，道：“看什么看，我是感染者。”"
                    ;
                const QString code =
                    "import jieba\n"
                    "s='" + text + "'\n"
                    "l=jieba.cut(s)\n"
                    "print('/'.join(l))\n"
                    ;
                for (const auto& e : Python_accessor) {
                    const auto p = e.second.get();
                    QSignalSpy MoreResult_signal_spy(p, &wmm::PythonAccessor::MoreResult);
                    QSignalSpy NoMoreResult_signal_spy(p, &wmm::PythonAccessor::NoMoreResult);
                    const auto conn = QObject::connect(p, &wmm::PythonAccessor::NoMoreResult, [&]() {
                        const QRegularExpression re("/");
                        QVERIFY(re.match(result).hasMatch());
                        for (const auto c : text) { QVERIFY(result.contains(c)); }
                        });
                    p->Execute(code);
                    QObject::disconnect(conn);
                    result.clear();
                }
            }
            {
                const QString text =
                    "一般認為，粵語的發展經歷了五個時期。"
                    "一、雛形期出現於秦漢，中原漢語開始進入嶺南地區。"
                    "二、成長期出現於魏晉南北朝，中原人第一次大規模南遷，拉近了古粵語與中原漢語的差別。"
                    "三、定型期出現於唐宋。隨着中原人第二次大規模南遷，粵語開始成為一種既能對應中古漢語發音（如《廣韻》）而又擁有獨立語言體系的語言。"
                    "四、獨立發展期始於清初。中原漢語急劇地向北方官話的方向發展而處在南方的粵語則繼續平穩而緩慢地按照自己獨特的規律發展。"
                    "滿清中末期，粵語首次逆向傳播到中原，這是由於當時的廣州成為唯一能接觸外來的人和物的地區。"
                    "於此同時，粵人大量移民海外，致使粵語開始向外傳播。"
                    "五、平行發展期出現於上世紀中葉，粵語開始演變為廣州粵語和香港粵語等方言片。"
                    ;
                const QString code =
                    "import jiagu\n"
                    "s='" + text + "'\n"
                    "print(jiagu.keywords(s,5))\n"
                    ;
                for (const auto& e : Python_accessor) {
                    const auto p = e.second.get();
                    QSignalSpy MoreResult_signal_spy(p, &wmm::PythonAccessor::MoreResult);
                    QSignalSpy NoMoreResult_signal_spy(p, &wmm::PythonAccessor::NoMoreResult);
                    const auto conn = QObject::connect(p, &wmm::PythonAccessor::NoMoreResult, [&]() {
                        result.remove(QRegularExpression(R"([\[\]'])"));
                        const auto words = result.split(',');
                        for (const auto& word : words) { QVERIFY(text.contains(word)); }
                        });
                    p->Execute(code);
                    QObject::disconnect(conn);
                    result.clear();
                }
            }
            {
                const QString text = 
                    "所有人都有一种紧迫感，因为没被灾难波及的区域已经越来越小了，再这样下去，现代文明将会毁灭殆尽，人类社会很快就要全面退回到中世纪。"
                    "在没有电流的情况下，最好的情况也就是重新进入蒸汽时代，事实上已经有很多人在着手制造大型蒸汽机了。"
                    "有人甚至断言，再过几十年，或许用蒸汽驱动的计算机也会被制造出来，"
                    "毕竟人类已经完全知道了计算机的工作原理，现在需要的只是将各种电子逻辑门用机械装置替代而已。"
                    "那时，天上也会重新出现各式飞艇，而燃煤所产生的滚滚黑烟也将重新笼罩在所有城市的上空。"
                    "而这笨重污浊、不可持续的文明，恐怕也将是人类文明最后的回光返照了吧。当燃煤和所有的化石能源耗尽，一切都将戛然而止。"
                    ;
                const QString code =
                    "import textrank4zh\n"
                    "s = '" + text + "'\n"
                    "summarizer = textrank4zh.TextRank4Sentence()\n"
                    "summarizer.analyze(text = s, lower = True, source = 'all_filters')\n"
                    "for item in summarizer.get_key_sentences(num = 3) :\n"
                    " print(item.index, item.weight, item.sentence)\n"
                    ;
                for (const auto& e : Python_accessor) {
                    const auto p = e.second.get();
                    QSignalSpy MoreResult_signal_spy(p, &wmm::PythonAccessor::MoreResult);
                    QSignalSpy NoMoreResult_signal_spy(p, &wmm::PythonAccessor::NoMoreResult);
                    const auto conn = QObject::connect(p, &wmm::PythonAccessor::NoMoreResult, [&]() {
                        const auto lines = result.split(QRegularExpression(R"((\r|\n|\r\n))"));
                        for (const auto& line : lines) {
                            const QRegularExpression re(R"([0-9]+\s+[0-9.]+)");
                            const auto match = re.match(line);
                        }
                        });
                    p->Execute(code);
                    QObject::disconnect(conn);
                    result.clear();
                }
            }            
            { // jionlp doesn't support Python 3.9 and later
                const QString text = 
                    "这是金融危机的第二年，人们本来以为危机已快要结束了，没想到只是开始，"
                    "所以社会处于一种焦躁的情绪中，每个人都需要发泄，并积极创造发泄的方式，诅咒的诞生也许与这种氛围有关。"
                    "诅咒的作者是一个女孩儿，18岁至28岁之间，关于她后来的IT考古学家们能知道的就这么多。"
                    "诅咒的对象是一个男孩儿，20岁，他的情况却都记载的很清楚，他叫撒碧，在太原工业大学上大四。"
                    "他和那女孩儿之间发生的事儿没什么特别的，也就是少男少女之间每天都在发生的那些个事儿，"
                    "后来有上千个版本，这里面可能有一个版本是真实的，但人们不知道是哪一个。"
                    "反正他们之间的事情都结束后，那女孩儿对那男孩儿是恨透了，于是编写了诅咒1.0。"
                    ;
                const QString code =
                    "import jionlp\n"
                    "s = '" + text + "'\n"
                    "sentiment_analyzer = jionlp.sentiment.LexiconSentiment()\n"
                    "print(sentiment_analyzer(s))\n"
                    ;
                const auto p = Python_accessor.at("3.8").get();
                QSignalSpy MoreResult_signal_spy(p, &wmm::PythonAccessor::MoreResult);
                QSignalSpy NoMoreResult_signal_spy(p, &wmm::PythonAccessor::NoMoreResult);
                const auto conn = QObject::connect(p, &wmm::PythonAccessor::NoMoreResult, [&]() {

                    });
                p->Execute(code);
                QObject::disconnect(conn);
                result.clear();
            }
        }
    }

    void PythonInteractor__basic() {
        namespace wmm = WritingMaterialsManager;

        //wmm::PythonInteractor Python_interactor;
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
