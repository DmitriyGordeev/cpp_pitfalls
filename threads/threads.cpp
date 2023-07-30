#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using std::cout;
using std::endl;

#include <windows.h>
#include <processthreadsapi.h>      // makes function GetCurrentProcessorNumber() accessible

std::mutex iomutex;


class Actor {
public:
    Actor() { cout << "Actor::constructor()\n"; }
    ~Actor() { cout << "Actor::destructor()\n"; }

    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

protected:
    int m_id {0};
};


class GameThread {
public:
    GameThread() {}

    ~GameThread() {
        if (m_thread.joinable())
            m_thread.join();
    }

    void start() {
        m_thread = std::thread([this]() {
            while(true) {
                tick();
            }
        });
    }

    void tick() {
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
        m_numTicksPassed++;

        if (m_numTicksPassed == 3) {
            m_actor = new Actor();

            iomutex.lock();
            cout << "GameThread::Adding actor to the scene, m_actor = " << m_actor << endl;
            iomutex.unlock();
        }

        if (m_numTicksPassed == 10) {
            gcActor();

            iomutex.lock();
            cout << "GameThread::garbage collecting actor, m_actor = " << m_actor << endl;
            iomutex.unlock();
        }

        iomutex.lock();
        cout << "GameThread::running tick " << m_numTicksPassed << endl;
        iomutex.unlock();
    }

    void addToScene(Actor* actor) {
        if (actor)
            m_actor = actor;
    }

    void gcActor() {
        if (!m_actor)
            return;

        iomutex.lock();
        cout << "GameThread::start garbage collecting the actor" << endl;
        iomutex.unlock();

        // начинаем удалять Actor, притворяясь как-будто это занимает некоторое время
        // и строка m_actor = nullptr выполняется не сразу,
        // во время этого RenderingThread может читать Actor или записывать, что приведет к ошибке
        // или невалидным значениям, так как m_actor = nullptr еще не выполнилась,
        // а значит проверка if(m_actor) будет бесполезна
        delete m_actor;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        m_actor = nullptr;

        iomutex.lock();
        cout << "GameThread::finishing garbage collecting the actor" << endl;
        iomutex.unlock();
    }

    Actor* getActor() const {
        return m_actor;
    }

protected:
    Actor* m_actor {nullptr};
    std::thread m_thread;
    int m_numTicksPassed {0};
};



class RenderingThread {
public:
    RenderingThread(GameThread* gameThread): m_GameThread(gameThread) {}

    ~RenderingThread() {
        if (m_thread.joinable())
            m_thread.join();
    }

    void start() {
        m_thread = std::thread([this]() {
            while(true) {
                tick();
            }
        });
    }

    void tick() {
        if (m_GameThread && m_numTicksPassed == 11) {

            Actor* actor = m_GameThread->getActor();
            if (actor) {
                iomutex.lock();
                cout << "RenderingThread: actor has id = " << actor->getId() << " <------ POSSIBLY GARBAGE NUMBER, because Rendering Thread is accessing object that began garbage collected" << endl;
                iomutex.unlock();

                // вызов функции setId() происходит из RenderingThread,
                // хотя объект принадлежит GameThread
                actor->setId(1);

                iomutex.lock();
                cout << "RenderingThread: setting new id for actor" << endl;
                iomutex.unlock();
            }
            else {
                iomutex.lock();
                cout << "RenderingThread: Actor is not valid!" << endl;
                iomutex.unlock();
            }

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
        m_numTicksPassed++;

        iomutex.lock();
        cout << "RenderingThread::running tick " << m_numTicksPassed << endl;
        iomutex.unlock();
    }

protected:
    std::thread m_thread;
    int m_numTicksPassed {0};
    GameThread* m_GameThread;
};



int main() {

    GameThread gameThread;
    gameThread.start();

    RenderingThread renderingThread(&gameThread);
    renderingThread.start();


    return 0;
}
