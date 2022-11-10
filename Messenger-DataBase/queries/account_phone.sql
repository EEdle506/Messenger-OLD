create table account_phone (
    phoneID int not null auto_increment,
    accountID int not null,  
    phoneNumber varchar(11) not null,
    phoneCountry varchar(2)  not null,
    phoneCountryNumbers varchar(3) not null,
    primary key(phoneID),
    constraint FK_PHONE_accountID foreign key(accountID) 
    references account(accountID)
)