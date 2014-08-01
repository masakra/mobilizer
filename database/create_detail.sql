
DROP TABLE IF EXISTS "mobi"."detail";

CREATE TABLE "mobi"."detail" (
	number		char( 11 ) NOT NULL,
	start		timestamp with time zone NOT NULL,
	pair		varchar( 64 ) NOT NULL,
	type		varchar( 16 ) NOT NULL,
	durfact		varchar( 16 ) NOT NULL,	-- фактическое время / трафик
	duration	varchar( 16 ) NOT NULL,	-- время / трафик для оплаты
	bill		numeric( 12, 4 ) NOT NULL
);

CREATE INDEX detail_number_idx ON "mobi"."detail" USING btree ( number );
CREATE INDEX detail_start_idx ON "mobi"."detail" USING btree ( start );
