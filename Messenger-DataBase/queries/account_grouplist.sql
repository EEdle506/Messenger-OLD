create table account_grouplist (
    grouplistID int not null auto_increment,
    acccountID int not null,
    groupHashSum varchar(64) not null,
    primary key(grouplistID),
    constraint FK_ACCOUNTgroulIst_accountID foreign key(accountID) 
    references account(accountID)
)