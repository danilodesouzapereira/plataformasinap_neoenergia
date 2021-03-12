SELECT
  IIF(ISNULL(MAX(intObraId)), 0, MAX(intObraId)) + 1 AS IdProx 
FROM 
  AlternativaObra
WHERE 
  intEstudoId = [p_EstudoId] AND
  intAlternativaId = [p_AlternativaId]
 