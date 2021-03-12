SELECT
  IIF(ISNULL(MAX(intAlternativaId)), 0, MAX(intAlternativaId)) + 1 AS IdProx 
FROM 
  EstudoAlternativa
WHERE 
  intEstudoId = [p_EstudoId]
 