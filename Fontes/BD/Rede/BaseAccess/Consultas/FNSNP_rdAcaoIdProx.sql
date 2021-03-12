SELECT
  IIF(ISNULL(MAX(intAcaoId)), 0, MAX(intAcaoId)) + 1 AS IdProx 
FROM 
  ObraAcao
WHERE 
  intEstudoId = [p_EstudoId] AND
  intAlternativaId = [p_AlternativaId] AND
  intObraId = [p_ObraId]
 