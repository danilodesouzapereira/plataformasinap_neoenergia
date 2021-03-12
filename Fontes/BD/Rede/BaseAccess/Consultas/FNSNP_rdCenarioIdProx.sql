SELECT
  IIF(ISNULL(MAX(intCenarioId)), 0, MAX(intCenarioId)) + 1 AS IdProx 
FROM 
  Cenario
