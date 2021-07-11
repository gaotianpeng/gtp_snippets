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
    virtual void undo() = 0;

protected:
    Command() {}
};

class LightOnCommand: public Command {
public:
    LightOnCommand(Light* light):light_(light) {}

    void execute() {
        light_->on();
    }

    void undo() {
        light_->off();
    }

private:
    Light* light_;
};

class LightOffCommand: public Command {
public:
    LightOffCommand(Light* light):light_(light) {}

    void execute() {
        light_->off();
    }

    void undo() {
        light_->on();
    }

private:
    Light* light_;
};

class NoCommand: public Command {
public:
    virtual void execute() {}
    virtual void undo() {}
};

class SimpleRemoteControl {
public:
    void setCommand(Command* cmd) {
        command_ = cmd;
    }

    void onButtonWasPressed() {
        command_->execute();
        undo_cmd_ = command_;
    }

    void offButtonWasPressed() {
        command_->execute();
        undo_cmd_ = command_;
    }

    void undoButtonWasPushed() {
        undo_cmd_->undo();
    }

private:
    Command* command_;
    Command* undo_cmd_;
};

int main(int argc, char* argv[]) {
    SimpleRemoteControl control;
    Light light;
    LightOnCommand cmd(&light);

    control.setCommand(&cmd);
    control.onButtonWasPressed();
    control.undoButtonWasPushed();
    control.offButtonWasPressed();
    control.undoButtonWasPushed();

    return 0;
}
