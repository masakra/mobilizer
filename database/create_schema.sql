DROP SCHEMA IF EXISTS "mobi" CASCADE;

CREATE SCHEMA "mobi";

CREATE TABLE "mobi"."tarif" (
	id			SERIAL UNIQUE NOT NULL,
	caption		varchar( 128 ) NOT NULL,
	"limit"		numeric( 12, 4 ) NOT NULL CHECK ( "limit" >= 0 )
);

CREATE TABLE "mobi"."number" (
	number		char( 11 ) NOT NULL,
	people_id	int REFERENCES "common"."people" ( id ),
	post		varchar( 255 ),
	pseudo		varchar( 64 ),
	start		timestamp without time zone NOT NULL DEFAULT '1970-01-01 00:00:00',
	otdel_id	int REFERENCES "erp"."otdel" ( id ),
	tarif_id	int REFERENCES "mobi"."tarif" ( id ),
	"limit"		numeric( 12, 4 ) NOT NULL DEFAULT 0 CHECK ( "limit" >= 0 ),
	city_iata	char( 3 ) REFERENCES "common"."city" ( iata ) ON UPDATE CASCADE,
	UNIQUE( number, start )
);

COMMENT ON TABLE "mobi"."number" IS 'Телефонные номера';

CREATE TABLE "mobi"."montly" (
	year		int NOT NULL CHECK ( year >= 1970 ),
	month		smallint NOT NULL CHECK ( month BETWEEN 1 AND 12 ),
	number		char( 11 ) NOT NULL,
	bill		numeric( 12, 4 ) NOT NULL,
	UNIQUE( year, month, number )
);

