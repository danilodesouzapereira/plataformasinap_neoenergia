SELECT
  IIF(ISNULL(MAX(intTipoConsId)), 0, MAX(intTipoConsId)) + 1 AS IdProx 
FROM 
  TipoConsumidor
 