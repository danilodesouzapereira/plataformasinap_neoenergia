SELECT
  IIF(ISNULL(MAX(intTipoRedeId)), 0, MAX(intTipoRedeId)) + 1 AS IdProx 
FROM 
  TipoRede
 