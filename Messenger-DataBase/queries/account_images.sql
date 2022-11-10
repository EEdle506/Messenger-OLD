create table account_images (
    picID int not null auto_increment,
    accountID int not null,
    picture mediumblob not null,
    currentPic tinyint default true,
    primary key(picID),
    constraint FK_PROFIMAGE_accountID foreign key(accountID) 
    references account(accountID)
)