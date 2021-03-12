  SELECT
    Rede.intRedeId                                                            AS RedeId,
    Rede.intTipoRedeId                                                        AS TipoRedeId,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    IIF(ISNULL(Rede.intAgrupamentoId),       -1, Rede.intAgrupamentoId)       AS AgrupamentoId,
    Rede.intMRedeId                                                           AS MRedeId,
    Rede.bolRedutivel                                                         AS Redutivel,
    Rede.bolEstatica                                                          AS Estatica,
    Rede.bolRadial                                                            AS Radial,
    Rede.bolEquivalente                                                       AS Equivalente,
    Rede.dttRevisao                                                           AS Revisao,
    Rede.intColorId                                                           AS ColorId,
    Rede.intBarraIniId                                                        AS BarraIniId,
    Rede.intEsqX1                                                             AS EsqX1,
    Rede.intEsqY1                                                             AS EsqY1,
    Rede.intEsqX2                                                             AS EsqX2,
    Rede.intEsqY2                                                             AS EsqY2,
    Rede.intUtmX1                                                             AS UtmX1,
    Rede.intUtmY1                                                             AS UtmY1,
    Rede.intUtmX2                                                             AS UtmX2,
    Rede.intUtmY2                                                             AS UtmY2,
    Equipamento.strCodigo                                                     AS RedeCodigo,
    Equipamento.strExtId                                                      AS RedeExtId,
    Equipamento.intShow                                                       AS Show,
    TipoRede.strCodigo                                                        AS TipoRedeCodigo,
    TipoRede.strDescricao                                                     AS TipoRedeDescricao,
    Agrupamento.strCodigo                                                     AS AgrupamentoCodigo,
    IIF(ISNULL(RedeDominio.intDominioId), -1, RedeDominio.intDominioId)       AS DominioId
  FROM
    (
    (
    (
    (
     Rede
      INNER JOIN Equipamento ON
        Rede.intRedeId = Equipamento.intEquipId AND
        Rede.intEstudoId = Equipamento.intEstudoId
    )

      INNER JOIN TipoRede ON
        Rede.intTipoRedeId = TipoRede.intTipoRedeId
    )
      LEFT OUTER JOIN Agrupamento ON
         IIF(ISNULL(Rede.intEstudoId), -1, Rede.intEstudoId) = Agrupamento.intEstudoId AND
         IIF(ISNULL(Rede.intAgrupamentoId), -1, Rede.intAgrupamentoId) = Agrupamento.intAgrupamentoId
    )

      LEFT OUTER JOIN RedeDominio ON
         IIF(ISNULL(Rede.intRedeId), -1, Rede.intRedeId) = RedeDominio.intRedeId
    )
  WHERE
    Rede.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    (( [p_TipoRedeId] = -1) OR (Rede.intTipoRedeId = [p_TipoRedeId] ));
