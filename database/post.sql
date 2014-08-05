CREATE TABLE "erp"."post" (
	id		SERIAL UNIQUE NOT NULL,
	caption	varchar( 255 ) UNIQUE NOT NULL
);

COMMENT ON TABLE "erp"."post" IS 'Должности';
