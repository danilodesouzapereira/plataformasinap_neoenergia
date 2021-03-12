INSERT INTO TmpEquipCurva (intEquipId, intConjCurvaId)
SELECT
  Equipamento.intEquipId,
  IIF(ISNULL(intConjCurvaId), -1, intConjCurvaId)
FROM
  RRTrafo RREquip INNER JOIN Equipamento ON 
    RREquip.intEstudoId = Equipamento.intEstudoId AND    
    RREquip.intRRTrafoId = Equipamento.intEquipId
WHERE
   RREquip.intEstudoId = [p_EstudoId] AND
   RREquip.intMRedeId = [p_MRedeId]
