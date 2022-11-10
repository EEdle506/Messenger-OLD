create table devices(
	deviceID int not null auto_increment,
    accountID int not null,
    deviceHash varchar(64) not null,
    deviceName varchar(16) not null,
    deviceLocIp varchar(15) not null,
    deviceMain bool not null,
    primary key(deviceID),
    constraint FK_DEVICE_accountID foreign key(accountID) 
    references account(accountID)
)