DELETE 
  Corte.*
FROM 
  Corte INNER JOIN TmpEquip ON
    Corte.intMRedeId = TmpEquip.intEquipId
WHERE 
  Corte.intEstudoId = [p_EstudoId];
