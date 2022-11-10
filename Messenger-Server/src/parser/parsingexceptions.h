#pragma once

#include <QException>

#include <string>

class _CommandException
    : public QException
{
    std::string m_Problem;

public:
    _CommandException(const char* message)
    {
        m_Problem.append("[ERROR] ").append(message + '\n');
    }

    const char* what() const noexcept override {
        return m_Problem.c_str();
    }

    void raise() const override { throw *this; }
    _CommandException *clone() const override { return new _CommandException(*this); }
};
