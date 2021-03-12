DELETE 
  RREquipamentoBarra.*
FROM 
  RREquipamentoBarra INNER JOIN TmpEquip ON
    RREquipamentoBarra.intMRedeId = TmpEquip.intEquipId
WHERE 
  RREquipamentoBarra.intEstudoId = [p_EstudoId];