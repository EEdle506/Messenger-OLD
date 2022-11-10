create table group_participant (
    ID int not null auto_increment,
    groupID int not null,
    accountID int not null,
    joinDate timestamp default CURRENT_TIMESTAMP,
    primary key(ID),
    FOREIGN KEY (groupID) 
    REFERENCES group_info(groupID)
)