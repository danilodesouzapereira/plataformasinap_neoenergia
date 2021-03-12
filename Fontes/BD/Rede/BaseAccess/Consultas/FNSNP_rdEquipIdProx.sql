SELECT 
  IIF(ISNULL(MAX(intEquipId)), 0, MAX(intEquipId)) + 1 AS IdProx 
FROM 
  Equipamento
WHERE
  intEstudoId = [p_EstudoId] 
