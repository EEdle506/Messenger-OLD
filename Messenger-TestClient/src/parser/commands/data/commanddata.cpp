#include "commanddata.h"

_CommandData::_CommandData() {}

/*

    Command

*/
_CommandData::_CommandData(Messenger::Command command, const std::initializer_list<Messenger::Parameter>& parameters)
    : v_Parameters(parameters)
{
    v_CommandData = std::make_pair(command, Messenger::EnumToString(command));
}

QString _CommandData::strCommand() const
{
    return v_CommandData.second;
}

Messenger::Command _CommandData::enumCommand() const
{
    return  v_CommandData.first;
}

_ParameterData _CommandData::parameters() const
{
    return v_Parameters;
}

void _CommandData::clear()
{
    v_CommandData.second.clear();
    v_Parameters.clear();
}
