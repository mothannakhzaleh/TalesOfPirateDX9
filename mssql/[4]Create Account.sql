USE AccountServer;
INSERT INTO account_login (name, password) VALUES ('admin', '327E7E3821F5F6D33C090137F979BF48EE62E9051C1610E1D6468ECB3C67A124');
USE GameDB;
INSERT INTO account (ato_id, ato_nome, jmes) VALUES ((SELECT ISNULL(MAX(ato_id) + 1,1) FROM account), 'admin', 99);
