#include "parser.h"

#include "parsingexceptions.h"
#include "commands/data/parseddata.h"

CommandParser::CommandParser(QObject* parent) :
    QObject(parent) {}

CommandParser::~CommandParser() {}

void CommandParser::inputCommand(const QByteArray &data)
{
    m_incomingRow = data;

    try {
        runParsing();
    }
    catch(const _CommandException& ex) {
        qDebug() << ex.what();
    }
}

void CommandParser::runParsing()
{
    using namespace Messenger;

    if (m_incomingRow.isEmpty())
        throw _CommandException("Message is empty");

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

    for (auto const& [info, method] : m_Commands) {
        if (info.strCommand() == *splitedStr.begin()) {
            command = info;
            commandFunction = method;
        }
    }
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
    commandFunction->setParameters(data);
    emit dataParsed(commandFunction);
}
