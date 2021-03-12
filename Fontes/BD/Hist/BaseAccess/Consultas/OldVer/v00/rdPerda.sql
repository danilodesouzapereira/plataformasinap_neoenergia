SELECT 
   TipoRede.*, 
   Perda.*, 
   IIF(ISNULL(GrupoPerdaRede.intGrupoId), 3, GrupoPerdaRede.intGrupoId) AS GrupoPerdaRedeId
FROM 
  (
  (
  TipoRede INNER JOIN Perda ON 
    TipoRede.TipoRede_id = Perda.TipoRede_id
  )
  LEFT OUTER JOIN GrupoPerdaRede ON 
    Perda.Rede_id = GrupoPerdaRede.intRedeId
  )
WHERE 
  Perda.data = [p_DataRef]
ORDER BY 
  Perda.Rede_id;
