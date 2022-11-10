#pragma once

#include "parameterdata.h"

class _CommandData
{
    std::pair<Server::Command, QString> v_CommandData;
    _ParameterData v_Parameters;

public:
    _CommandData();
    _CommandData(Server::Command command, const std::initializer_list<Server::Parameter>& parameters);

    QString strCommand() const;
    Server::Command enumCommand() const;
    _ParameterData parameters() const;

    bool isEmpty() const { return v_CommandData.second.isEmpty(); }
    void clear();

    operator Server::Command() const {
        return v_CommandData.first;
    }
    operator QString() const {
        return v_CommandData.second;
    }
    operator std::string() const {
        return v_CommandData.second.toStdString();
    }
};
