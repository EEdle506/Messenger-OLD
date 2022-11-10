create table chat(
	chatID int not null auto_increment,
    accountID int not null,
    chatFriendHash varchar(32) not null,
    primary key(chatID),
    FOREIGN KEY (accountID) REFERENCES account(accountID)
)