create table group_messages (
    ID int not null auto_increment,
    participantID int not null,
    msg varchar(2048) not null,
    msgDate timestamp default CURRENT_TIMESTAMP
    primary key(groupmsgID),
    foreign key(participantID) 
    references group_participant(participantID)
)