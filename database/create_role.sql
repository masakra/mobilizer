DROP ROLE IF EXISTS "mobi_user";

CREATE ROLE "mobi_user";

GRANT USAGE ON SCHEMA "mobi" TO "mobi_user";
GRANT USAGE ON SCHEMA "common" TO "mobi_user";
GRANT USAGE ON SCHEMA "erp" TO "mobi_user";

GRANT SELECT, INSERT, UPDATE, DELETE ON "mobi"."number" TO "mobi_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON "mobi"."tarif" TO "mobi_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON "mobi"."montly" TO "mobi_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON "mobi"."detail" TO "mobi_user";

GRANT SELECT, INSERT, UPDATE, DELETE ON "common"."people" TO "mobi_user";
GRANT ALL ON SEQUENCE "common"."people_id_seq" TO "mobi_user";
GRANT SELECT ON "common"."city" TO "mobi_user";

GRANT SELECT ON "erp"."department" TO "mobi_user";
GRANT SELECT ON "erp"."otdel" TO "mobi_user";
