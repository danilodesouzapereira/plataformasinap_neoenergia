SELECT
  IIF(ISNULL(COUNT(intEquipId)), 0, COUNT(intEquipId)) AS TipoEquipCount
FROM 
  Equipamento
WHERE
  intEstudoId = [p_EstudoId] AND
  intTipoEquipId = [p_TipoEquipId]
 

