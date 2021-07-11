#include <iostream>
using namespace std;

class Light {
public:
    void on() {
        cout << "Light turned on!" << endl;
    }

    void off() {
        cout << "Light truned off!" << endl;
    }
};

class Command {
public:
    virtual void execute() = 0;

protected:
    Command() {}
};

class LightOnCommand: public Command {
public:
    LightOnCommand(Light* light):light_(light) {}

    void execute() {
        light_->on();
    }

private:
    Light* light_;
};

class SimpleRemoteControl {
public:
    void setCommand(Command* cmd) {
        command_ = cmd;
    }

    void buttonPressed() {
        command_->execute();
    }

private:
    Command* command_;
};

int main(int argc, char* argv[]) {
    SimpleRemoteControl control;
    Light light;
    LightOnCommand cmd(&light);

    control.setCommand(&cmd);
    control.buttonPressed();

    return 0;
}
