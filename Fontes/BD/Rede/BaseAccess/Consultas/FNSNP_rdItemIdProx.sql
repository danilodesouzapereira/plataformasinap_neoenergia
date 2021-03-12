SELECT
  IIF(ISNULL(MAX(intItemId)), 0, MAX(intItemId)) + 1 AS IdProx 
FROM 
  ObraAcao
WHERE 
  intEstudoId = [p_EstudoId] AND
  intAlternativaId = [p_AlternativaId] AND
  intObraId = [p_ObraId]
 
 