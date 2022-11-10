CREATE TABLE group_info(
    groupID int not null AUTO_INCREMENT,
    groupTag varchar(32) not null,
    groupName varchar(24) not null,
    groupOwnerSum varchar(64) not null,
    groupCreationDate timestamp not null,
    groupHashSum varchar(64) not null,
    primary key(groupID),
    FOREIGN KEY (accountID) 
    REFERENCES account(accountID)
)