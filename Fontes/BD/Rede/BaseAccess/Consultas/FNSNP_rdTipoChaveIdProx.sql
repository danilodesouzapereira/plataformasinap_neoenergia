SELECT
  IIF(ISNULL(MAX(intTipoChaveId)), 0, MAX(intTipoChaveId)) + 1 AS IdProx 
FROM 
  TipoChave
 