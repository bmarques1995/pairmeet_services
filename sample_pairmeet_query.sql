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

select * from Users;

show tables;

rollback;

commit;