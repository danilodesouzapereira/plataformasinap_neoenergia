    SELECT
      Equipamento.strCodigo                                                     AS Codigo,
      Equipamento.strExtId                                                      AS ExtId,
      IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
      IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
      IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
      Equipamento.bolAlteradoObra                                               AS AlteradoObra,
      Equipamento.intShow                                                       AS Show,
      EquipamentoBarra.intBarraId                                               AS BarraId,
      EquipamentoBarra.fltPosBar                                                AS PosBar,
      EquipamentoBarra.intDX                                                    AS DX,
      EquipamentoBarra.intDY                                                    AS DY,
      EqAux.intCargaNLId                                                        AS CargaNLId,
      EqAux.intOrdemId                                                          AS OrdemId,
      EqAux.fltFatorPot                                                         AS FatorPot,
      EqAux.bolIndutivo                                                         AS Indutivo,
      EqAux.bolkVA                                                              AS kVA,
      EqAux.fltSNomA                                                            AS SNomA,
      EqAux.fltSNomB                                                            AS SNomB,
      EqAux.fltSNomC                                                            AS SNomC,
      EqAux.fltAngA                                                             AS AngA,
      EqAux.fltAngB                                                             AS AngB,
      EqAux.fltAngC                                                             AS AngC
    FROM
      (
      (
      (
      Equipamento
        INNER JOIN EquipamentoBarra ON
          Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
          Equipamento.intEquipId = EquipamentoBarra.intEquipId
       )

        INNER JOIN CargaNL EqAux ON
          Equipamento.intEquipId = EqAux.intCargaNLId AND
          Equipamento.intEstudoId = EqAux.intEstudoId
       )

       INNER JOIN BarraRede ON
          EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND
          EquipamentoBarra.intBarraId = BarraRede.intBarraId
       )

    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      Equipamento.intTipoEquipId = 34 AND
      BarraRede.intRedeId = [p_RedeId]
    ORDER BY
      Equipamento.intEquipId,
      EqAux.intOrdemId;
