CREATE DATABASE dbservermsg;

create table account(
    accountID int not null AUTO_INCREMENT,
    accountTag varchar(32) not null,
    accountName varchar(16) not null,
    accountEmail varchar(255),
    accountPass varchar(64) not null,
    accountAbout varchar(70),
    accountDate timestamp default CURRENT_TIMESTAMP
    accountHashSum varchar(64) not null,
    primary key(accountID)
)