create table group_pictures(
    ID int not null auto_increment,
    participantID int not null,
    picture mediumblob not null,
    picDate datetime default CURRENT_TIMESTAMP,
    primary key(grouppicID),
    foreign key(participantID) 
    references group_participant(participantID)
)