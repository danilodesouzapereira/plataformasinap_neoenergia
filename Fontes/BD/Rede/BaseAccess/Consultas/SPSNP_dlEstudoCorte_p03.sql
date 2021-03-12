DELETE 
  RRTrafo.*
FROM 
  RRTrafo INNER JOIN TmpEquip ON
    RRTrafo.intMRedeId = TmpEquip.intEquipId
WHERE 
  RRTrafo.intEstudoId = [p_EstudoId];