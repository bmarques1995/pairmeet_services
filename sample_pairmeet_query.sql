create schema pairmeet;

#https://stackoverflow.com/questions/3191664/list-of-all-locales-and-their-short-codes

use pairmeet;

start transaction;

create table Users(
	Name VARCHAR(100) NOT NULL,
    Email VARCHAR(255) PRIMARY KEY,
    Password BINARY(64) NOT NULL,
    VerifiedEmail bool NOT NULL DEFAULT false,
    Locale ENUM
    (
    'en_US',
    'fr_CA',
    'es_MX',
    'ja_JP',
    'pt_BR'
    ) NOT NULL DEFAULT 'en_US'
);

desc Users;

drop table Users;

insert into Users (Name, Email, Password, VerifiedEmail, Locale)
VALUES ("B Marques", "testebash@outlook.com", 0x38e05c33d7b067127f217d8c856e554fcff09c9320b8a5979ce2ff5d95dd27ba35d1fba50c562dfd1d6cc48bc9c5baa4390894418cc942d968f97bcb659419ed, false, 'pt_BR');

select * from Users;

delete from Users where Email="contaandroidbruno1995@gmail.com";
delete from Users where Email="testebash@outlook.com";

update Users set VerifiedEmail=true where email="testebash@outlook.com"; 

show tables;

rollback;

commit;