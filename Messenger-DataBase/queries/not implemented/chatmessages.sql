create table chatmessages(
	chatmsgID int not null auto_increment,
    chatID int not null,
    chatmsgSenderHash varchar(64) not null,
    chatMessage varchar(2048) not null,
    chatmsgDate datetime default CURRENT_TIMESTAMP,
    primary key(chatmsgID),
    constraint FK_CHATMSG_chatID foreign key(chatID) 
    references chatdata(chatID)
)