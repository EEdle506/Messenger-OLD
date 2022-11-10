#include "commandparser.h"

#include "parsingexceptions.h"
#include "parseddata.h"

_CommandParser::_CommandParser(QObject* parent) :
    QObject(parent) {}

_CommandParser::~_CommandParser() {}

void _CommandParser::inputCommand(const QByteArray &data)
{
    m_incomingRow = data;

    try {
        runParsing();
    }
    catch(const _CommandException& ex) {
        qCritical() << ex.what();
    }
}

void _CommandParser::runParsing()
{
    using namespace Server;

    if (m_incomingRow.isEmpty())
        throw _CommandException("Message was empty");
    // Cheking if that was command
    if (m_incomingRow[0] != '/')
        throw _CommandException("Can not find slash");
    m_incomingRow.remove(0, 1);

    // Splits command into parts
    QByteArrayList splitedStr = m_incomingRow.split(' ');
    if (splitedStr.first().isEmpty())
        throw _CommandException("Can not find command");
   
    // Command info and function
    _CommandData command;
    _BaseCommand* commandFunction;

    for (auto const& [data, method] : m_Commands) {
        if (data.strCommand() == *splitedStr.begin()) {
            command = data;
            commandFunction = method;
            break;
        }
    }
    qDebug() << "[INFO] To excute command" << splitedStr.first();
    // Deleting command in row
    splitedStr.erase(splitedStr.begin());

    // parsed data
    _ParsedData data;
    Parameter oldParameter;
    bool oldData = false;
    for (const auto& dataPart : splitedStr)
    {
        const auto& parameters = command.parameters().mappedParameters();
        qsizetype j = 0;
        for (const auto& [str, enm] : parameters)
        {
            if (dataPart.contains(str.toUtf8()))
            {
                qsizetype posEq = dataPart.indexOf('=');
                if (posEq != -1)
                {
                    oldParameter = enm;
                    if (!oldData) data.addParameter(oldParameter);
                    data.addDatatoParameter(oldParameter, dataPart.right(dataPart.size() - posEq - 1));
                    oldData = true;
                    break;
                }
            }
            ++j;
        }
        if (j == parameters.size() && !oldData) 
            data.addDatatoParameter(oldParameter, dataPart);
        oldData = false;
    }

    if (data.size() == 0) 
        throw _CommandException("No parameters was parsed");

    commandFunction->setParameters(data);
    emit dataParsed(commandFunction);
}
