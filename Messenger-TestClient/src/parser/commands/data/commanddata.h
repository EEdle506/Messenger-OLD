#pragma once

#include "parameterdata.h"

class _CommandData
{
    std::pair<Messenger::Command, QString> v_CommandData;
    _ParameterData v_Parameters;

public:
    _CommandData();
    _CommandData(Messenger::Command command, const std::initializer_list<Messenger::Parameter>& parameters);

    QString strCommand() const;
    Messenger::Command enumCommand() const;
    _ParameterData parameters() const;

    bool isEmpty() const { return v_CommandData.second.isEmpty(); }
    void clear();

    operator Messenger::Command() const {
        return v_CommandData.first;
    }
    operator QString() const {
        return v_CommandData.second;
    }
    operator std::string() const {
        return v_CommandData.second.toStdString();
    }
};
