SELECT
  IIF(ISNULL(GrupoPerda.intGrupoId)   , -1, GrupoPerda.intGrupoId)        AS GrupoId,
  IIF(ISNULL(GrupoPerda.intGrupoPaiId), -1, GrupoPerda.intGrupoPaiId)     AS GrupoPaiId,
  GrupoPerda.strDescricao                                                 AS Descricao,
  GrupoPerda.intTipoGrupoRedeId                                           AS TipoGrupoId
FROM
  GrupoPerda
ORDER BY
  GrupoPerda.intTipoGrupoRedeId ASC,
  GrupoPerda.intGrupoId         ASC
  